#pragma once

#include <stdint.h>
#include "io.h"
inline bool keyboard_ready()
{
    return inb(0x64) & 1;
}


bool isalpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// based on US QWERTY Scancode set 1 - what is emulated in QEMU
// https://wiki.osdev.org/PS/2_Keyboard
enum class Scancode {
    ESC_PRESSED = 0x01, ONE_PRESSED = 0x02,
    TWO_PRESSED = 0x03, THREE_PRESSED = 0x04,
    FOUR_PRESSED = 0x05, FIVE_PRESSED = 0x06,
    SIX_PRESSED = 0x07, SEVEN_PRESSED = 0x08,
    EIGHT_PRESSED = 0x09, NINE_PRESSED = 0x0A,
    ZERO_PRESSED = 0x0B, DASH_PRESSED = 0x0C,
    EQ_PRESSED = 0x0D, BACKSPACE_PRESSED = 0x0E,
    TAB_PRESSED = 0x0F, Q_PRESSED = 0x10,
    W_PRESSED = 0x11, E_PRESSED = 0x12,
    R_PRESSED = 0x13, T_PRESSED = 0x14,
    Y_PRESSED = 0x15, U_PRESSED = 0x16,
    I_PRESSED = 0x17, O_PRESSED = 0x18,
    P_PRESSED = 0x19, OPEN_SQUARE_BRACKET_PRESSED = 0x1A,
    CLOSE_SQUARE_BRACKET_PRESSED = 0x1B, ENTER_PRESSED = 0x1C,
    LCTRL_PRESSED = 0x1D, A_PRESSED = 0x1E,
    S_PRESSED = 0x1F, D_PRESSED = 0x20,
    F_PRESSED = 0x21, G_PRESSED = 0x22,
    H_PRESSED = 0x23, J_PRESSED = 0x24,
    K_PRESSED = 0x25, L_PRESSED = 0x26,
    SEMICOLON_PRESSED = 0x27, SINGLE_QUOTE_PRESSED = 0x28,
    BACKTICK_PRESSED = 0x29, LSHIFT_PRESSED = 0x2A,
    BACKSLASH_PRESSED = 0x2B, Z_PRESSED = 0x2C,
    X_PRESSED = 0x2D, C_PRESSED = 0x2E,
    V_PRESSED = 0x2F, B_PRESSED = 0x30,
    N_PRESSED = 0x31, M_PRESSED = 0x32,
    COMMA_PRESSED = 0x33, DOT_PRESSED = 0x34,
    SLASH_PRESSED = 0x35, RSHIFT_PRESSED = 0x36,
    NUMPAD_STAR_PRESSED = 0x37, LALT_PRESSED = 0x38,
    SPACE_PRESSED = 0x39, CAPSLOCK_PRESSED = 0x3A,
    F1_PRESSED = 0x3B, F2_PRESSED = 0x3C,
    F3_PRESSED = 0x3D, F4_PRESSED = 0x3E,
    F5_PRESSED = 0x3F, F6_PRESSED = 0x40,
    F7_PRESSED = 0x41, F8_PRESSED = 0x42,
    F9_PRESSED = 0x43, F10_PRESSED = 0x44,
    NUMLOCK_PRESSED = 0x45, SCROLLOCK_PRESSED = 0x46,
    NUMPAD_7_PRESSED = 0x47, NUMPAD_8_PRESSED = 0x48,
    NUMPAD_9_PRESSED = 0x49, NUMPAD_DASH_PRESSED = 0x4A,
    NUMPAD_4_PRESSED = 0x4B, NUMPAD_5_PRESSED = 0x4C,
    NUMPAD_6_PRESSED = 0x4D, NUMPAD_PLUS_PRESSED = 0x4E,
    NUMPAD_1_PRESSED = 0x4F, NUMPAD_2_PRESSED = 0x50,
    NUMPAD_3_PRESSED = 0x51, NUMPAD_0_PRESSED = 0x52,
    NUMPAD_DOT_PRESSED = 0x53, F11_PRESSED = 0x57,
    F12_PRESSED = 0x58, ESC_RELEASED = 0x81,
    LSHIFT_RELEASED = 0xAA, RSHIFT_RELEASED = 0xB6,
    EXTEND = 0xE0
};

enum class ExtendedScancode {
    CURSOR_UP_PRESSED = (uint8_t) Scancode::NUMPAD_8_PRESSED,
    CURSOR_DOWN_PRESSED = (uint8_t) Scancode::NUMPAD_2_PRESSED,
    CURSOR_LEFT_PRESSED = (uint8_t) Scancode::NUMPAD_4_PRESSED,
    CURSOR_RIGHT_PRESSED = (uint8_t) Scancode::NUMPAD_6_PRESSED
};

typedef union IncomingScancode {
    Scancode basic;
    ExtendedScancode extended;
    uint8_t raw;
} IncomingScancode;

inline IncomingScancode keyboard_Scancode()
{
    IncomingScancode incoming;
    incoming.raw = inb(0x60);
    return incoming;
}

/*
 * If the Scancode s can be reasonably represented as an ASCII value,
 * return that ASCII value.
 * Else, return \0
 */
char Scancode_to_ascii(Scancode s)
{
    switch(s) {
        case Scancode::ESC_PRESSED:
            return 0x1B;
        case Scancode::NUMPAD_1_PRESSED:
        case Scancode::ONE_PRESSED:
            return '1';
        case Scancode::NUMPAD_2_PRESSED:
        case Scancode::TWO_PRESSED:
            return '2';
        case Scancode::NUMPAD_3_PRESSED:
        case Scancode::THREE_PRESSED:
            return '3';
        case Scancode::NUMPAD_4_PRESSED:
        case Scancode::FOUR_PRESSED:
            return '4';
        case Scancode::NUMPAD_5_PRESSED:
        case Scancode::FIVE_PRESSED:
            return '5';
        case Scancode::NUMPAD_6_PRESSED:
        case Scancode::SIX_PRESSED:
            return '6';
        case Scancode::NUMPAD_7_PRESSED:
        case Scancode::SEVEN_PRESSED:
            return '7';
        case Scancode::NUMPAD_8_PRESSED:
        case Scancode::EIGHT_PRESSED:
            return '8';
        case Scancode::NUMPAD_9_PRESSED:
        case Scancode::NINE_PRESSED:
            return '9';
        case Scancode::NUMPAD_0_PRESSED:
        case Scancode::ZERO_PRESSED:
            return '0';
        case Scancode::DASH_PRESSED:
            return '-';
        case Scancode::EQ_PRESSED:
            return '=';
        case Scancode::BACKSPACE_PRESSED:
            return '\b';
        case Scancode::Q_PRESSED:
            return 'q';
        case Scancode::W_PRESSED:
            return 'w';
        case Scancode::E_PRESSED:
            return 'e';
        case Scancode::R_PRESSED:
            return 'r';
        case Scancode::T_PRESSED:
            return 't';
        case Scancode::Y_PRESSED:
            return 'y';
        case Scancode::U_PRESSED:
            return 'u';
        case Scancode::I_PRESSED:
            return 'i';
        case Scancode::O_PRESSED:
            return 'o';
        case Scancode::P_PRESSED:
            return 'p';
        case Scancode::OPEN_SQUARE_BRACKET_PRESSED:
            return '[';
        case Scancode::CLOSE_SQUARE_BRACKET_PRESSED:
            return ']';
        case Scancode::ENTER_PRESSED:
            return '\n';
        case Scancode::A_PRESSED:
            return 'a';
        case Scancode::S_PRESSED:
            return 's';
        case Scancode::D_PRESSED:
            return 'd';
        case Scancode::F_PRESSED:
            return 'f';
        case Scancode::G_PRESSED:
            return 'g';
        case Scancode::H_PRESSED:
            return 'h';
        case Scancode::J_PRESSED:
            return 'j';
        case Scancode::K_PRESSED:
            return 'k';
        case Scancode::L_PRESSED:
            return 'l';
        case Scancode::SEMICOLON_PRESSED:
            return ';';
        case Scancode::SINGLE_QUOTE_PRESSED:
            return '\'';
        case Scancode::BACKTICK_PRESSED:
            return '`';
        case Scancode::Z_PRESSED:
            return 'z';
        case Scancode::X_PRESSED:
            return 'x';
        case Scancode::C_PRESSED:
            return 'c';
        case Scancode::V_PRESSED:
            return 'v';
        case Scancode::B_PRESSED:
            return 'b';
        case Scancode::N_PRESSED:
            return 'n';
        case Scancode::M_PRESSED:
            return 'm';
        case Scancode::COMMA_PRESSED:
            return ',';
        case Scancode::DOT_PRESSED:
            return '.';
        case Scancode::SLASH_PRESSED:
            return '/';
        case Scancode::NUMPAD_STAR_PRESSED:
            return '*';
        case Scancode::NUMPAD_PLUS_PRESSED:
            return '+';
        case Scancode::NUMPAD_DASH_PRESSED:
            return '-';
        case Scancode::NUMPAD_DOT_PRESSED:
            return '.';
        case Scancode::SPACE_PRESSED:
            return ' ';
        default:
            return '\0';
    }
}

typedef enum press_state {
    PRESSED,
    RELEASED
} press_state;

press_state get_press_state(Scancode s)
{
    if((uint8_t)s < 0x81) {
        return PRESSED;
    } else {
        return RELEASED;
    }
}

enum class keyboard_special_type {
    UNKNOWN,
    LSHIFT,
    RSHIFT,
    LALT,
    RALT,
    CURSOR_UP,
    CURSOR_DOWN,
    CURSOR_LEFT,
    CURSOR_RIGHT
};

class keyboard_input {
    public:
        bool is_ascii:1;
        press_state pressed:1;
        union {
            char ascii;
            keyboard_special_type special_value;
        };
        keyboard_input(Scancode s)
        {
            char ascii = Scancode_to_ascii(s);
            is_ascii = !!ascii;
            pressed = get_press_state(s);
            if(ascii) {
                this->ascii = ascii;
                return;
            }
            switch(s) {
                case Scancode::LSHIFT_PRESSED:
                    special_value = keyboard_special_type::LSHIFT;
                    break;
                case Scancode::LSHIFT_RELEASED:
                    special_value = keyboard_special_type::LSHIFT;
                    break;
                case Scancode::RSHIFT_PRESSED:
                    special_value = keyboard_special_type::RSHIFT;
                    break;
                case Scancode::RSHIFT_RELEASED:
                    special_value = keyboard_special_type::RSHIFT;
                    break;
                default:
                    special_value = keyboard_special_type::UNKNOWN;
                    break;
            }
        }
        keyboard_input(ExtendedScancode s)
        {
            switch(s) {
                case ExtendedScancode::CURSOR_UP_PRESSED:
                    special_value = keyboard_special_type::CURSOR_UP;
                    pressed = PRESSED;
                    break;
                case ExtendedScancode::CURSOR_DOWN_PRESSED:
                    special_value = keyboard_special_type::CURSOR_DOWN;
                    pressed = PRESSED;
                    break;
                case ExtendedScancode::CURSOR_LEFT_PRESSED:
                    special_value = keyboard_special_type::CURSOR_LEFT;
                    pressed = PRESSED;
                    break;
                case ExtendedScancode::CURSOR_RIGHT_PRESSED:
                    special_value = keyboard_special_type::CURSOR_RIGHT;
                    pressed = PRESSED;
                    break;
                default:
                    special_value = keyboard_special_type::UNKNOWN;
                    break;
            }
        }
        constexpr bool isShift() const
        {
            return special_value == keyboard_special_type::LSHIFT || special_value == keyboard_special_type::RSHIFT;
        }
};


keyboard_input next_input()
{
    while(!keyboard_ready());
    IncomingScancode incoming = keyboard_Scancode();
    if(incoming.basic == Scancode::EXTEND) {
        while(!keyboard_ready());
        return keyboard_input(keyboard_Scancode().extended);
    }
    return keyboard_input(incoming.basic);
}
