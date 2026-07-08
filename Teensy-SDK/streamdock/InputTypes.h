#pragma once

#include <cstdint>
#include <cstddef>

enum class EventType {
    BUTTON,
    KNOB_ROTATE,
    KNOB_PRESS,
    SWIPE,
    TOUCH_POINT,
    DIP_SWITCH,
    UNKNOWN,
};

// Teensyduino defines KEY_0..KEY_9 in keylayouts.h as USB keycodes.
#if defined(KEY_0)
#undef KEY_0
#endif
#if defined(KEY_1)
#undef KEY_1
#endif
#if defined(KEY_2)
#undef KEY_2
#endif
#if defined(KEY_3)
#undef KEY_3
#endif
#if defined(KEY_4)
#undef KEY_4
#endif
#if defined(KEY_5)
#undef KEY_5
#endif
#if defined(KEY_6)
#undef KEY_6
#endif
#if defined(KEY_7)
#undef KEY_7
#endif
#if defined(KEY_8)
#undef KEY_8
#endif
#if defined(KEY_9)
#undef KEY_9
#endif

enum class ButtonKey : int {
    KEY_1 = 1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_10,
    KEY_11,
    KEY_12,
    KEY_13,
    KEY_14,
    KEY_15,
    KEY_16,
    KEY_17,
    KEY_18,
    KEY_19,
    KEY_20,
    KEY_21,
    KEY_22,
    KEY_23,
    KEY_24,
    KEY_25,
    KEY_26,
    KEY_27,
    KEY_28,
    KEY_29,
    KEY_30,
    KEY_31,
    KEY_32,
};

enum class KnobId {
    KNOB_1,
    KNOB_2,
    KNOB_3,
    KNOB_4,
};

enum class DIPSwitchId {
    DIP_1,
    DIP_2,
};

enum class Direction {
    LEFT,
    RIGHT,
};

struct InputEvent {
    EventType event_type = EventType::UNKNOWN;
    ButtonKey key = ButtonKey::KEY_1;
    bool has_key = false;
    KnobId knob_id = KnobId::KNOB_1;
    bool has_knob_id = false;
    DIPSwitchId dip_id = DIPSwitchId::DIP_1;
    bool has_dip_id = false;
    Direction direction = Direction::LEFT;
    bool has_direction = false;
    int state = 0;
    int x = 0;
    int y = 0;
    bool has_touch = false;
    const uint8_t *raw_data = nullptr;
    size_t raw_data_len = 0;

    static InputEvent unknown() {
        return InputEvent{};
    }

    static InputEvent button(ButtonKey key, int state) {
        InputEvent event;
        event.event_type = EventType::BUTTON;
        event.key = key;
        event.has_key = true;
        event.state = state;
        return event;
    }

    static InputEvent knobPress(KnobId knob_id, int state) {
        InputEvent event;
        event.event_type = EventType::KNOB_PRESS;
        event.knob_id = knob_id;
        event.has_knob_id = true;
        event.state = state;
        return event;
    }

    static InputEvent knobRotate(KnobId knob_id, Direction direction) {
        InputEvent event;
        event.event_type = EventType::KNOB_ROTATE;
        event.knob_id = knob_id;
        event.has_knob_id = true;
        event.direction = direction;
        event.has_direction = true;
        return event;
    }

    static InputEvent swipe(Direction direction) {
        InputEvent event;
        event.event_type = EventType::SWIPE;
        event.direction = direction;
        event.has_direction = true;
        return event;
    }

    static InputEvent dipSwitch(DIPSwitchId dip_id, int state, Direction direction, bool has_direction) {
        InputEvent event;
        event.event_type = EventType::DIP_SWITCH;
        event.dip_id = dip_id;
        event.has_dip_id = true;
        event.state = state;
        event.direction = direction;
        event.has_direction = has_direction;
        return event;
    }

    static InputEvent touchPoint(int x, int y, const uint8_t *raw_data, size_t raw_data_len) {
        InputEvent event;
        event.event_type = EventType::TOUCH_POINT;
        event.x = x;
        event.y = y;
        event.has_touch = true;
        event.raw_data = raw_data;
        event.raw_data_len = raw_data_len;
        return event;
    }
};
