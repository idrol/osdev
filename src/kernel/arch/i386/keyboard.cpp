#include "kernel/Keyboard.h"
#include "kernel/IO.h"
#include "kernel/tty.h"
#include "kernel/ScancodeSet1.h"

void(*Keyboard::key_pressed)(uint8_t) = nullptr;
ScancodeSet Keyboard::scancodeSet;

void keyboard_irq() {
    uint8_t nativeScancode = IO::inb(0x60);
    uint8_t scancode = Keyboard::scancodeSet.decode(nativeScancode);
    Keyboard::KeyPressed(scancode);
}

void Keyboard::Initialize() {
    ScancodeSet1 set = ScancodeSet1();
    set.Initialize();
    scancodeSet = set;
}

void Keyboard::KeyPressed(uint8_t scancode) {
    if(key_pressed != nullptr)
        (*key_pressed)(scancode);
}

void Keyboard::RegisterKeyPressedListener(void(*listener)(uint8_t scancode)) {
    key_pressed = listener;
}