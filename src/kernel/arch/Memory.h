#pragma once
#include "grub/multiboot.h"
#include "common/stdint.h"

struct MemoryMap {
    multiboot_memory_map_t *map;
    uint32_t length;
    uint32_t size;
};

struct MemorySegment {
    MemorySegment* prev;
    MemorySegment* next;
    size_t size; // Size of the memory segment
    bool allocated;
} __attribute__((packed));

class Memory {
    public:
        // The offset is added to the address 0x100000 that is the start of the upper memory area
        // but because the kernel is also located here we have to add an offset to not overlap dynamic memory with the kernel itself
        // to be safe this should generaly be 0x300000
        static void Initialize(multiboot_info_t *mbd, uint32_t upperMemoryOffset); // Detects memory map
        static void PrintMemoryMap();
        // Returns a pointer to the allocated space of the specified size. Returns nullptr incase no space was found
        static void* malloc(size_t size);
        static void free(void* ptr);
        static size_t GetAllocatedMemory();
        static size_t GetAvailableMemory();

    private:
        static MemoryMap memoryMap;
        // Upper memory size in KB
        static size_t upperMemorySize;
        static MemorySegment* firstSegment;
        static size_t availableMemory;
        static size_t allocatedMemory;
};