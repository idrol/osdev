#include "kernel/ScancodeSet.h"
#include "kernel/tty.h"
#include "kernel/IO.h"

uint8_t ScancodeSet::decode(uint8_t scancode) {
    return scancodeMappings[scancode];
}