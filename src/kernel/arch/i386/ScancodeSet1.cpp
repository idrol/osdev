#include "kernel/ScancodeSet1.h"
#include "kernel/tty.h"

void ScancodeSet1::Initialize() {
    ScancodeSet::Initialize();
    
    scancodeMappings[0x01] = KEY_ESCAPE;
    scancodeMappings[0x02] = KEY_1;
    scancodeMappings[0x03] = KEY_2;
    scancodeMappings[0x04] = KEY_3;
    scancodeMappings[0x05] = KEY_4;
    scancodeMappings[0x06] = KEY_5;
    scancodeMappings[0x07] = KEY_6;
    scancodeMappings[0x08] = KEY_7;
    scancodeMappings[0x09] = KEY_8;
    scancodeMappings[0x0A] = KEY_9;
    scancodeMappings[0x0B] = KEY_0;
    scancodeMappings[0x0C] = KEY_MINUS;
    scancodeMappings[0x0D] = KEY_EQUALS;
    scancodeMappings[0x0E] = KEY_BACKSPACE;
    scancodeMappings[0x0F] = KEY_TAB;
    scancodeMappings[0x10] = KEY_Q;
    scancodeMappings[0x11] = KEY_W;
    scancodeMappings[0x12] = KEY_E;
    scancodeMappings[0x13] = KEY_R;
    scancodeMappings[0x14] = KEY_T;
    scancodeMappings[0x15] = KEY_Y;
    scancodeMappings[0x16] = KEY_U;
    scancodeMappings[0x17] = KEY_I;
    scancodeMappings[0x18] = KEY_O;
    scancodeMappings[0x19] = KEY_P;
    scancodeMappings[0x1A] = KEY_LEFT_BRACKET;
    scancodeMappings[0x1B] = KEY_RIGHT_BRACKET;
    scancodeMappings[0x1C] = KEY_ENTER;
    scancodeMappings[0x1D] = KEY_LEFT_CTRL;
    scancodeMappings[0x1E] = KEY_A;
    scancodeMappings[0x1F] = KEY_S;
    scancodeMappings[0x20] = KEY_D;
    scancodeMappings[0x21] = KEY_F;
    scancodeMappings[0x22] = KEY_G;
    scancodeMappings[0x23] = KEY_H;
    scancodeMappings[0x24] = KEY_J;
    scancodeMappings[0x25] = KEY_K;
    scancodeMappings[0x26] = KEY_L;
    scancodeMappings[0x27] = KEY_SEMICOLON;
    scancodeMappings[0x28] = KEY_SINGLEQUOTE;
    scancodeMappings[0x29] = KEY_BACKTICK;
    scancodeMappings[0x2A] = KEY_LEFT_SHIFT;
    scancodeMappings[0x2B] = KEY_BACKSLASH;
    scancodeMappings[0x2C] = KEY_Z;
    scancodeMappings[0x2D] = KEY_X;
    scancodeMappings[0x2E] = KEY_C;
    scancodeMappings[0x2F] = KEY_V;
    scancodeMappings[0x30] = KEY_B;
    scancodeMappings[0x31] = KEY_N;
    scancodeMappings[0x32] = KEY_M;
    scancodeMappings[0x33] = KEY_COMMA;
    scancodeMappings[0x34] = KEY_DOT;
    scancodeMappings[0x35] = KEY_FORWARDSLASH;
    scancodeMappings[0x36] = KEY_RIGHT_SHIFT;
    scancodeMappings[0x37] = KEY_KEYPAD_MULTIPLY;
    scancodeMappings[0x38] = KEY_LEFT_ALT;
    scancodeMappings[0x39] = KEY_SPACE;
    scancodeMappings[0x3A] = KEY_CAPS_LOCK;
    scancodeMappings[0x3B] = KEY_F1;
    scancodeMappings[0x3C] = KEY_F2;
    scancodeMappings[0x3D] = KEY_F3;
    scancodeMappings[0x3E] = KEY_F4;
    scancodeMappings[0x3F] = KEY_F5;
    scancodeMappings[0x40] = KEY_F6;
    scancodeMappings[0x41] = KEY_F7;
    scancodeMappings[0x42] = KEY_F8;
    scancodeMappings[0x43] = KEY_F9;
    scancodeMappings[0x44] = KEY_F10;
    scancodeMappings[0x45] = KEY_NUMLOCK;
    scancodeMappings[0x46] = KEY_SCROLLOCK;

}