#pragma once
#include "common/stdint.h"
#include "kernel/arch/ScancodeSet.h"

/**
 * Scan code set 1 primary key pressed codes from 0x01->0x58
 * Scane code set 1 primary ke released offset 0x80 from pressed codes.
 */

void keyboard_irq();

class Keyboard {
    public:
        static void Initialize();
        static void RegisterKeyPressedListener(void(*listener)(uint8_t scancode));
        static void KeyPressed(uint8_t scancode); // Invokes key pressed listener
        static void(*key_pressed)(uint8_t scancode);
        static ScancodeSet scancodeSet;
};