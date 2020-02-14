#pragma once

#include "common/stdint.h"

extern "C" {
    char* itoa(uint32_t value, char * str, int base);
    // Check if num is aligned to kibValue
    // ex. isKibAligned(0x100000, 4) tests if 0x100000 is 4KiB aligned
    bool isKiBAligned(uint32_t num, uint32_t kibValue);
}