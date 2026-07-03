"""
StreamDock input event type system

Provides unified input event definitions, including buttons, knobs, and swipe gestures.
"""

from dataclasses import dataclass
from typing import Optional
from json import loads

class LedInfo():
    mode: int = 0
    speed: int = 0
    flag: int = 0
    hsv: tuple[int, int, int] = (0, 0, 0)
    base_hs: tuple[int, int] = (0, 0)

    def __init__(self, dict_data: dict):
        self.__dict__ = dict_data
        self.hsv = tuple(self.__dict__["hsv"])
        self.base_hs = tuple(self.__dict__["base_hs"])

@dataclass
class DeviceConfigEvent:
    """
    Unified device configuration event class

    """
    version: str = ""
    os: str = ""
    scr: int = 0
    style: int = 0
    Stream_Dock: str = ""
    led_info: LedInfo = None

    def __init__(self, json_data: str):
        self.__dict__ = loads(json_data)
        self.led_info = LedInfo(self.__dict__["led_info"])

    def __str__(self):
        return f"DeviceConfigEvent(version={self.version}, os={self.os}, screen={self.scr}, style={self.style}, stream_dock={self.Stream_Dock}, led_info={self.led_info})"


