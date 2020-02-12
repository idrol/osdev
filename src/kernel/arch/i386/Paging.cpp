#include "kernel/arch/Paging.h"

uint32_t Paging::pageDirectory[1024];

void Paging::Initialize() {
    for(int i = 0; i < 1024; i++) {
        pageDirectory[i] = 0x00000002;
    }
}