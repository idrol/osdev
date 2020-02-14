#include "kernel/arch/Paging.h"


// Bit 5 for accessed is set by the cpu if the cpu reads or writes to this page but it's up to the kernel to clear this flag or simply ignore it
// If present is 0 then all other bits are ignored and free to be used by the kernel
// Bit 31..12 page table 4kb address 11..9 unused bits free to kernel, 8 ignored, 7 page size(0 for 4kb), 6 always 0, 5 Accesed, 4 cache disabled, 3 write through, 2 user or supervisor, 1 read or write, 0 present
uint32_t Paging::pageDirectory[1024];
uint32_t Paging::first_page_table[1024];
uint32_t Paging::second_page_table[1024];

extern "C" void loadPageDirectory(size_t *);
extern "C" void enablePaging();

void Paging::Initialize() {
    for(int i = 0; i < 1024; i++) {
        pageDirectory[i] = 0x00000002;
    }
    for(int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 3;
        second_page_table[i] = ((i+1024) * 0x1000) | 3;
    }
    pageDirectory[0] = ((uint32_t)first_page_table) | 3; // Identity map the first 4MiB
    //pageDirectory[1] = ((uint32_t)second_page_table) | 3; // Identity map the second 4MiB

    loadPageDirectory(pageDirectory);
    enablePaging();
}