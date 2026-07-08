#include "DeviceKeyMaps.h"

namespace DeviceKeyMaps {

int normalizeState(int state) {
    return state == 0x01 ? 1 : 0;
}

int getImageKey(const KeyMapEntry *map, size_t count, ButtonKey logical_key) {
    for (size_t i = 0; i < count; ++i) {
        if (map[i].logical == logical_key) {
            return map[i].hardware;
        }
    }
    return -1;
}

bool hwToLogical(const KeyMapEntry *map, size_t count, int hardware_code, ButtonKey &logical_key) {
    for (size_t i = 0; i < count; ++i) {
        if (map[i].hardware == hardware_code) {
            logical_key = map[i].logical;
            return true;
        }
    }
    return false;
}

InputEvent decodeButtonMap(const KeyMapEntry *map, size_t count, int hardware_code, int state) {
    ButtonKey logical_key;
    if (hwToLogical(map, count, hardware_code, logical_key)) {
        return InputEvent::button(logical_key, normalizeState(state));
    }
    return InputEvent::unknown();
}

InputEvent decodeKnobRotate(const KnobRotateEntry *map, size_t count, int hardware_code) {
    for (size_t i = 0; i < count; ++i) {
        if (map[i].hardware == hardware_code) {
            return InputEvent::knobRotate(map[i].knob, map[i].direction);
        }
    }
    return InputEvent::unknown();
}

InputEvent decodeKnobPress(const KnobPressEntry *map, size_t count, int hardware_code, int state) {
    for (size_t i = 0; i < count; ++i) {
        if (map[i].hardware == hardware_code) {
            return InputEvent::knobPress(map[i].knob, normalizeState(state));
        }
    }
    return InputEvent::unknown();
}

const KeyMapEntry REMAP_15[] = {
    {ButtonKey::KEY_1, 11}, {ButtonKey::KEY_2, 12}, {ButtonKey::KEY_3, 13},
    {ButtonKey::KEY_4, 14}, {ButtonKey::KEY_5, 15}, {ButtonKey::KEY_6, 6},
    {ButtonKey::KEY_7, 7}, {ButtonKey::KEY_8, 8}, {ButtonKey::KEY_9, 9},
    {ButtonKey::KEY_10, 10}, {ButtonKey::KEY_11, 1}, {ButtonKey::KEY_12, 2},
    {ButtonKey::KEY_13, 3}, {ButtonKey::KEY_14, 4}, {ButtonKey::KEY_15, 5},
};
const size_t REMAP_15_COUNT = sizeof(REMAP_15) / sizeof(REMAP_15[0]);

const KeyMapEntry REMAP_293S_18[] = {
    {ButtonKey::KEY_1, 13}, {ButtonKey::KEY_2, 10}, {ButtonKey::KEY_3, 7},
    {ButtonKey::KEY_4, 4}, {ButtonKey::KEY_5, 1}, {ButtonKey::KEY_6, 14},
    {ButtonKey::KEY_7, 11}, {ButtonKey::KEY_8, 8}, {ButtonKey::KEY_9, 5},
    {ButtonKey::KEY_10, 2}, {ButtonKey::KEY_11, 15}, {ButtonKey::KEY_12, 12},
    {ButtonKey::KEY_13, 9}, {ButtonKey::KEY_14, 6}, {ButtonKey::KEY_15, 3},
    {ButtonKey::KEY_16, 16}, {ButtonKey::KEY_17, 17}, {ButtonKey::KEY_18, 18},
};
const size_t REMAP_293S_18_COUNT = sizeof(REMAP_293S_18) / sizeof(REMAP_293S_18[0]);

const KeyMapEntry REMAP_N4_14[] = {
    {ButtonKey::KEY_1, 11}, {ButtonKey::KEY_2, 12}, {ButtonKey::KEY_3, 13},
    {ButtonKey::KEY_4, 14}, {ButtonKey::KEY_5, 15}, {ButtonKey::KEY_6, 6},
    {ButtonKey::KEY_7, 7}, {ButtonKey::KEY_8, 8}, {ButtonKey::KEY_9, 9},
    {ButtonKey::KEY_10, 10}, {ButtonKey::KEY_11, 1}, {ButtonKey::KEY_12, 2},
    {ButtonKey::KEY_13, 3}, {ButtonKey::KEY_14, 4},
};
const size_t REMAP_N4_14_COUNT = sizeof(REMAP_N4_14) / sizeof(REMAP_N4_14[0]);

const KeyMapEntry REMAP_N4PRO_15[] = {
    {ButtonKey::KEY_1, 11}, {ButtonKey::KEY_2, 12}, {ButtonKey::KEY_3, 13},
    {ButtonKey::KEY_4, 14}, {ButtonKey::KEY_5, 15}, {ButtonKey::KEY_6, 6},
    {ButtonKey::KEY_7, 7}, {ButtonKey::KEY_8, 8}, {ButtonKey::KEY_9, 9},
    {ButtonKey::KEY_10, 10}, {ButtonKey::KEY_11, 1}, {ButtonKey::KEY_12, 2},
    {ButtonKey::KEY_13, 3}, {ButtonKey::KEY_14, 4}, {ButtonKey::KEY_15, 5},
};
const size_t REMAP_N4PRO_15_COUNT = sizeof(REMAP_N4PRO_15) / sizeof(REMAP_N4PRO_15[0]);

const KeyMapEntry REMAP_K1PRO[] = {
    {ButtonKey::KEY_1, 0x05}, {ButtonKey::KEY_2, 0x03}, {ButtonKey::KEY_3, 0x01},
    {ButtonKey::KEY_4, 0x06}, {ButtonKey::KEY_5, 0x04}, {ButtonKey::KEY_6, 0x02},
};
const size_t REMAP_K1PRO_COUNT = sizeof(REMAP_K1PRO) / sizeof(REMAP_K1PRO[0]);

const KeyMapEntry REMAP_XL_32[] = {
    {ButtonKey::KEY_1, 25}, {ButtonKey::KEY_2, 26}, {ButtonKey::KEY_3, 27},
    {ButtonKey::KEY_4, 28}, {ButtonKey::KEY_5, 29}, {ButtonKey::KEY_6, 30},
    {ButtonKey::KEY_7, 31}, {ButtonKey::KEY_8, 32}, {ButtonKey::KEY_9, 17},
    {ButtonKey::KEY_10, 18}, {ButtonKey::KEY_11, 19}, {ButtonKey::KEY_12, 20},
    {ButtonKey::KEY_13, 21}, {ButtonKey::KEY_14, 22}, {ButtonKey::KEY_15, 23},
    {ButtonKey::KEY_16, 24}, {ButtonKey::KEY_17, 9}, {ButtonKey::KEY_18, 10},
    {ButtonKey::KEY_19, 11}, {ButtonKey::KEY_20, 12}, {ButtonKey::KEY_21, 13},
    {ButtonKey::KEY_22, 14}, {ButtonKey::KEY_23, 15}, {ButtonKey::KEY_24, 16},
    {ButtonKey::KEY_25, 1}, {ButtonKey::KEY_26, 2}, {ButtonKey::KEY_27, 3},
    {ButtonKey::KEY_28, 4}, {ButtonKey::KEY_29, 5}, {ButtonKey::KEY_30, 6},
    {ButtonKey::KEY_31, 7}, {ButtonKey::KEY_32, 8},
};
const size_t REMAP_XL_32_COUNT = sizeof(REMAP_XL_32) / sizeof(REMAP_XL_32[0]);

const KeyMapEntry REMAP_MINI_6[] = {
    {ButtonKey::KEY_1, 1}, {ButtonKey::KEY_2, 2}, {ButtonKey::KEY_3, 3},
    {ButtonKey::KEY_4, 4}, {ButtonKey::KEY_5, 5}, {ButtonKey::KEY_6, 6},
};
const size_t REMAP_MINI_6_COUNT = sizeof(REMAP_MINI_6) / sizeof(REMAP_MINI_6[0]);

const KeyMapEntry REMAP_N3_9[] = {
    {ButtonKey::KEY_1, 1}, {ButtonKey::KEY_2, 2}, {ButtonKey::KEY_3, 3},
    {ButtonKey::KEY_4, 4}, {ButtonKey::KEY_5, 5}, {ButtonKey::KEY_6, 6},
    {ButtonKey::KEY_7, 0x25}, {ButtonKey::KEY_8, 0x30}, {ButtonKey::KEY_9, 0x31},
};
const size_t REMAP_N3_9_COUNT = sizeof(REMAP_N3_9) / sizeof(REMAP_N3_9[0]);

const KeyMapEntry REMAP_N1_17[] = {
    {ButtonKey::KEY_1, 1}, {ButtonKey::KEY_2, 2}, {ButtonKey::KEY_3, 3},
    {ButtonKey::KEY_4, 4}, {ButtonKey::KEY_5, 5}, {ButtonKey::KEY_6, 6},
    {ButtonKey::KEY_7, 7}, {ButtonKey::KEY_8, 8}, {ButtonKey::KEY_9, 9},
    {ButtonKey::KEY_10, 10}, {ButtonKey::KEY_11, 11}, {ButtonKey::KEY_12, 12},
    {ButtonKey::KEY_13, 13}, {ButtonKey::KEY_14, 14}, {ButtonKey::KEY_15, 15},
    {ButtonKey::KEY_16, 0x1E}, {ButtonKey::KEY_17, 0x1F},
};
const size_t REMAP_N1_17_COUNT = sizeof(REMAP_N1_17) / sizeof(REMAP_N1_17[0]);

const KeyMapEntry REMAP_M18_18[] = {
    {ButtonKey::KEY_1, 11}, {ButtonKey::KEY_2, 12}, {ButtonKey::KEY_3, 13},
    {ButtonKey::KEY_4, 14}, {ButtonKey::KEY_5, 15}, {ButtonKey::KEY_6, 6},
    {ButtonKey::KEY_7, 7}, {ButtonKey::KEY_8, 8}, {ButtonKey::KEY_9, 9},
    {ButtonKey::KEY_10, 10}, {ButtonKey::KEY_11, 1}, {ButtonKey::KEY_12, 2},
    {ButtonKey::KEY_13, 3}, {ButtonKey::KEY_14, 4}, {ButtonKey::KEY_15, 5},
    {ButtonKey::KEY_16, 0x25}, {ButtonKey::KEY_17, 0x30}, {ButtonKey::KEY_18, 0x31},
};
const size_t REMAP_M18_18_COUNT = sizeof(REMAP_M18_18) / sizeof(REMAP_M18_18[0]);

} // namespace DeviceKeyMaps
