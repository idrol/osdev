#pragma once
#include "common/stdint.h"

class Paging {
    public:
    static void Initialize();

    private:
    static uint32_t pageDirectory[1024] __attribute__((aligned(4096)));
    static uint32_t first_page_table[1024] __attribute__((aligned(4096)));
};