#pragma once
#include "common/stdint.h"

struct gdt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

class GDT {
public:
    static void Initialize();
    static void SetEntry(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

    private:
    static gdt_entry_t gdt_entries[5];
    static gdt_ptr_t gdt_ptr;
};