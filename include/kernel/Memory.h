#pragma once
#include "grub/multiboot.h"
#include <stdint.h>

class Memory {
    public:
        static void Initialize(multiboot_info_t *mbd); // Detects memory map

    private:
        static multiboot_memory_map_t *mmap;
        static uint32_t mmapLength;
};