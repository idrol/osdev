#pragma once
#include <stdint.h>
#include "kernel/Keys.h"

class ScancodeSet {
public:
    virtual void Initialize() {
        for(int i = 0; i < 256; i++) scancodeMappings[i] = 0x00;
    };
    uint8_t decode(uint8_t scancode);
protected:
    uint8_t scancodeMappings[256];
};