#!/usr/bin/env python3
"""
Convert images for Mirabox / StreamDock devices.

Resizes and rotates each source image per device format settings and writes files
to output_dir/<DeviceName>/key/, touchscreen/, and secondscreen/ (if supported).

Static images are saved as JPEG. Animated GIFs are saved as GIF with all frames
processed and original timing preserved.
"""

from __future__ import annotations

import argparse
import copy
import sys
from pathlib import Path

from PIL import Image, ImageSequence

from StreamDock.ImageHelpers.PILHelper import _to_native_format
from StreamDock.ProductIDs import (
    K1Pro,
    StreamDock293,
    StreamDock293V3,
    StreamDock293s,
    StreamDock293sV3,
    StreamDockM18,
    StreamDockM3,
    StreamDockMini,
    StreamDockN1,
    StreamDockN3,
    StreamDockN4,
    StreamDockN4Pro,
    StreamDockXL,
)

IMAGE_EXTENSIONS = {".jpg", ".jpeg", ".png", ".gif", ".bmp", ".webp", ".tif", ".tiff"}

DEVICE_CLASSES = {
    "K1Pro": K1Pro,
    "StreamDock293": StreamDock293,
    "StreamDock293V3": StreamDock293V3,
    "StreamDock293s": StreamDock293s,
    "StreamDock293sV3": StreamDock293sV3,
    "StreamDockM18": StreamDockM18,
    "StreamDockM3": StreamDockM3,
    "StreamDockMini": StreamDockMini,
    "StreamDockN1": StreamDockN1,
    "StreamDockN3": StreamDockN3,
    "StreamDockN4": StreamDockN4,
    "StreamDockN4Pro": StreamDockN4Pro,
    "StreamDockXL": StreamDockXL,
}

FORMAT_TARGETS = (
    ("key", "key_image_format"),
    #("touchscreen", "touchscreen_image_format"),
    #("secondscreen", "secondscreen_image_format"),
)


def list_devices() -> str:
    return ", ".join(sorted(DEVICE_CLASSES))


def resolve_devices(device_names: list[str]) -> list[str]:
    if not device_names:
        raise ValueError("Specify at least one device name or ALL.")

    if len(device_names) == 1 and device_names[0].upper() == "ALL":
        return sorted(DEVICE_CLASSES)

    unknown = [name for name in device_names if name not in DEVICE_CLASSES]
    if unknown:
        raise ValueError(
            f"Unknown device(s): {', '.join(unknown)}. "
            f"Available devices: {list_devices()}"
        )

    return device_names


def iter_source_images(input_dir: Path) -> list[Path]:
    if not input_dir.is_dir():
        raise FileNotFoundError(f"Input directory not found: {input_dir}")

    images = sorted(
        path
        for path in input_dir.iterdir()
        if path.is_file() and path.suffix.lower() in IMAGE_EXTENSIONS
    )
    if not images:
        raise FileNotFoundError(f"No images found in {input_dir}")
    return images


def get_device_formats(device_class) -> list[tuple[str, dict]]:
    stub = object()
    formats: list[tuple[str, dict]] = []

    for folder_name, method_name in FORMAT_TARGETS:
        method = getattr(device_class, method_name, None)
        if method is None:
            continue
        image_format = copy.deepcopy(method(stub))
        image_format["format"] = "JPEG"
        formats.append((folder_name, image_format))

    return formats


def is_animated_gif(path: Path) -> bool:
    if path.suffix.lower() != ".gif":
        return False
    with Image.open(path) as image:
        return getattr(image, "n_frames", 1) > 1


def convert_image(image: Image.Image, image_format: dict) -> Image.Image:
    fmt = copy.deepcopy(image_format)
    fmt["format"] = "JPEG"
    converted = _to_native_format(image.copy(), fmt)
    if converted.mode != "RGB":
        converted = converted.convert("RGB")
    return converted


def convert_gif_frame(frame: Image.Image, image_format: dict) -> Image.Image:
    fmt = copy.deepcopy(image_format)
    fmt["format"] = "PNG"
    return _to_native_format(frame.convert("RGBA"), fmt)


def save_animated_gif(
    source_path: Path,
    output_path: Path,
    image_format: dict,
) -> None:
    with Image.open(source_path) as image:
        frames: list[Image.Image] = []
        durations: list[int] = []

        for frame in ImageSequence.Iterator(image):
            converted = convert_gif_frame(frame, image_format)
            frames.append(converted.convert("RGB"))
            durations.append(max(1, int(frame.info.get("duration", 100))))

        loop = image.info.get("loop", 0)

    if not frames:
        raise ValueError(f"No frames found in animated GIF: {source_path}")

    frames[0].save(
        output_path,
        save_all=True,
        append_images=frames[1:],
        duration=durations,
        loop=loop,
        disposal=2,
    )


def convert_for_device(
    device_name: str,
    device_class,
    source_images: list[Path],
    output_dir: Path,
    jpeg_quality: int,
) -> int:
    device_formats = get_device_formats(device_class)
    if not device_formats:
        print(f"[skip] {device_name}: no image formats defined", flush=True)
        return 0

    written = 0
    for folder_name, image_format in device_formats:
        target_dir = output_dir / device_name / folder_name
        target_dir.mkdir(parents=True, exist_ok=True)

        size = image_format["size"]
        rotation = image_format["rotation"]
        print(
            f"[{device_name}/{folder_name}] "
            f"size={size[0]}x{size[1]} rotation={rotation}",
            flush=True,
        )

        for source_path in source_images:
            if is_animated_gif(source_path):
                output_path = target_dir / f"{source_path.stem}.gif"
                save_animated_gif(source_path, output_path, image_format)
            else:
                with Image.open(source_path) as image:
                    converted = convert_image(image, image_format)

                output_path = target_dir / f"{source_path.stem}.jpg"
                converted.save(output_path, "JPEG", quality=jpeg_quality)
            written += 1

    return written


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Resize and rotate images for Mirabox / StreamDock devices. "
            "Outputs JPEG files (or GIF for animated GIFs) to output_dir/<DeviceName>/."
        )
    )
    parser.add_argument("input_dir", type=Path, help="Directory containing source images")
    parser.add_argument("output_dir", type=Path, help="Directory for converted output")
    parser.add_argument(
        "devices",
        nargs="+",
        metavar="DEVICE",
        help=f"Device class name(s) to convert for, or ALL. Available: {list_devices()}",
    )
    parser.add_argument(
        "--quality",
        type=int,
        default=90,
        help="JPEG quality (1-100, default: 90)",
    )
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)

    if not 1 <= args.quality <= 100:
        print("JPEG quality must be between 1 and 100.", file=sys.stderr)
        return 2

    try:
        device_names = resolve_devices(args.devices)
        source_images = iter_source_images(args.input_dir)
    except (ValueError, FileNotFoundError) as exc:
        print(exc, file=sys.stderr)
        return 2

    args.output_dir.mkdir(parents=True, exist_ok=True)

    total_written = 0
    print(f"Converting {len(source_images)} image(s) for {len(device_names)} device(s)...")
    for device_name in device_names:
        total_written += convert_for_device(
            device_name,
            DEVICE_CLASSES[device_name],
            source_images,
            args.output_dir,
            args.quality,
        )

    print(f"Done. Wrote {total_written} file(s) under {args.output_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
