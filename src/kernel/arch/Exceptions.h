#pragma once
#include "common/stdint.h"

class Exceptions {
    public:
    static void Initialize();
    private:
    static void SetException(uint8_t exceptionNum, uint32_t addr);
};