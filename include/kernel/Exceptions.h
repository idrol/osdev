#pragma once
#include <stdint.h>

typedef unsigned int uword_t __attribute__ ((mode (__word__)));

class Exceptions {
    public:
    static void Initialize();
    private:
    static void SetException(uint8_t exceptionNum, uint32_t addr);
};