#include <stdbool.h>
#include <stddef.h>
#include "common/stdint.h"
#include "kernel/kernel.h"
#include "kernel/arch/tty.h"
#include "kernel/arch/GDT.h"
#include "kernel/arch/IDT.h"
#include "kernel/arch/IO.h"
#include "kernel/arch/Keyboard.h"
#include "kernel/KernelLib.h"
#include "kernel/arch/Memory.h"
#include "kernel/arch/Paging.h"
#include "kernel/arch/Exceptions.h"
#include "grub/multiboot.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "Current compiler is not a cross compiler"
#endif

extern "C" size_t kernel_start;
extern "C" size_t kernel_end;

void print_key(uint8_t scancode) {
    if(scancode == KEY_W) {
        char c = std::keycodeToChar(scancode);
        char c2[12];
        for(int i = 0; i < 12; i++) c2[i] = ' ';
        IO::itoa(scancode, c2, 16);
        Terminal::write(c2, 12);
        Terminal::write(&c, 1);
        Terminal::write(" was pressed\n", 13);
        Terminal::writestring("Key w was pressed\n");
    }
}

void boot_kernel(multiboot_info_t *mbd) {
    Terminal::initialize();
    Terminal::writeline("Initializing GDT");
    GDT::Initialize();
    Terminal::writeline("Initializing IDT");
    IDT::Initialize();
    Terminal::writeline("Initializing Interrupt handlers");
    IDT::SetDefaultIRQHandlers();
    Terminal::writeline("Setting up cpu exceptions");
    Exceptions::Initialize();
    Terminal::writeline("Initializing PIC");
    IDT::PIC_REMAP(32, 40);
    for(int i = 0; i < 16; i++) {
      IO::PIC_set_mask(i);
    }
    //IO::PIC_clear_mask(0); // Enable timer
    //IO::PIC_clear_mask(1);

    //Terminal::writeline("Enabling paging");
    //Paging::Initialize();

    Terminal::writeline("Enabling interrupts");
    asm volatile("sti");
    Terminal::writeline("Initializing Memory");
    Memory::Initialize(mbd, 0x300000); // Start memory managment at 0x400000

    Terminal::writeint32(Memory::GetAvailableMemory()/1024/1024);
    Terminal::writeline("MB of available memory");

    Terminal::writeint32(Memory::GetAllocatedMemory());
    Terminal::writeline("B allocated memory");

    Keyboard::Initialize();
    Terminal::writeline("Boot sequence completed");
}

void kernel_main(multiboot_info_t *mbd, uint32_t magic) {
    boot_kernel(mbd);

    Terminal::writestring("Kernal main location: ");
    Terminal::writehex32((uint32_t)&kernel_main);
    Terminal::newline();

#if 1
    Terminal::writestring("Kernel loaded at ");
    Terminal::writehex32((size_t)&kernel_start);
    Terminal::writestring(" to ");
    Terminal::writehex32((size_t)&kernel_end);
    uint32_t kernelSize = (size_t)&kernel_end - (size_t)&kernel_start;
    Terminal::writestring(" with a size of ");
    Terminal::writeint32(kernelSize);
    Terminal::writeline("b");
#endif

    #if 0
    multiboot_module_t* module = (multiboot_module_t*)mbd->mods_addr;
    Terminal::writestring("Loaded modules ");
    Terminal::writeint32(mbd->mods_count);
    Terminal::newline();
    Terminal::writeline("Loaded module");
    Terminal::writestring("Loaded at: ");
    Terminal::writehex32(module->mod_start);
    Terminal::writestring(" to: ");
    Terminal::writehex32(module->mod_end);
    Terminal::newline();
    #endif

#if 0
    Terminal::writeline("Attempting to alloc 1KB");
    void* ptr = Memory::malloc(1024);
    Terminal::writestring("Allocated memory att pointer ");
    Terminal::writehex32((uint32_t)ptr);
    Terminal::newline();
#endif

#if 0
    Terminal::writehex32(magic);
    Terminal::newline();
    Terminal::writehex32(mbd->flags);
    Terminal::newline();
#endif

    Keyboard::RegisterKeyPressedListener(&print_key);

    while(true) {
    
    }
}

void kernel_panic() {
    Terminal::writeline("Something went horibly wrong PANIC");
    asm volatile("hlt");
}