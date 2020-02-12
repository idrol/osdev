#pragma once
#include <stddef.h>
#include <stdint.h>

namespace std {
    size_t strlen(const char* str);
    char keycodeToChar(uint8_t keycode);
};