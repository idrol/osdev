#include "kernel/arch/ScancodeSet.h"
#include "kernel/arch/tty.h"
#include "kernel/arch/IO.h"

uint8_t ScancodeSet::decode(uint8_t scancode) {
    return scancodeMappings[scancode];
}