#pragma once
#include "common/stdint.h"

class Paging {
    public:
    static void Initialize();

    private:
    static uint32_t pageDirectory[1024] __attribute__((aligned(4096)));
};