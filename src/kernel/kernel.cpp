#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel/kernel.h"
#include "kernel/tty.h"
#include "kernel/GDT.h"
#include "kernel/IDT.h"
#include "kernel/IO.h"
#include "kernel/Keyboard.h"
#include "kernel/KernelLib.h"
#include "kernel/Memory.h"
#include "kernel/Exceptions.h"
#include "grub/multiboot.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "Current compiler is not a cross compiler"
#endif



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
  Terminal::writestring("Initializing GDT\n");
  GDT::Initialize();
  Terminal::writestring("Initializing IDT\n");
  IDT::Initialize();
  Terminal::writestring("Initializing Interrupt handlers\n");
  IDT::SetDefaultIRQHandlers();
  Terminal::writeline("Setting up cpu exceptions");
  Exceptions::Initialize();
  Terminal::writestring("Initializing PIC\n");
  IDT::PIC_REMAP(32, 40);
  for(int i = 0; i < 16; i++) {
    IO::PIC_set_mask(i);
  }
  //IO::PIC_clear_mask(0); // Enable timer
  //IO::PIC_clear_mask(1);

  Terminal::writestring("Enabling interrupts\n");
  asm volatile("sti");
  Terminal::writestring("Initializing Memory\n");
  Memory::Initialize(mbd);
  Keyboard::Initialize();
  Terminal::writestring("Boot sequence completed\n");
}

void kernel_main(multiboot_info_t *mbd, uint32_t magic) {
  boot_kernel(mbd);

  Terminal::writehex(magic);
  Terminal::newline();
  Terminal::writehex(mbd->flags);
  Terminal::newline();

  Keyboard::RegisterKeyPressedListener(&print_key);
  while(true) {
    
  }
}