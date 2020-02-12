#include "kernel/Memory.h"
#include "kernel/tty.h"

multiboot_memory_map_t *Memory::mmap;
uint32_t Memory::mmapLength;

void Memory::Initialize(multiboot_info_t *mbd) {
    Terminal::writehex(mbd->mmap_addr);
    Terminal::write("\n", 1);
    Terminal::writehex(mbd->mmap_length);
    Terminal::write("\n", 1);
    Terminal::writehex(mbd->mem_lower);
    Terminal::write("\n", 1);
    Terminal::writehex(mbd->mem_upper);
    Terminal::write("\n", 1);
    mmap = (multiboot_memory_map_t*)mbd->mmap_addr;
    for(mmap = (multiboot_memory_map_t*)mbd->mmap_addr;
        (uint32_t)mmap < (mbd->mmap_addr + mbd->mmap_length);
        mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size))) {
            //Terminal::writehex(mmap->addrH);
            Terminal::writehex(mmap->addrL);
            //Terminal::newline();
            //Terminal::writehex(mmap->lenH);
            Terminal::writehex(mmap->lenL);
            //Terminal::newline();
            Terminal::writehex(mmap->type);
            Terminal::newline();
    }
}
