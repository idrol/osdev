#include "kernel/arch/IDT.h"
#include "kernel/arch/IO.h"
#include "kernel/arch/tty.h"
#include "kernel/arch/Keyboard.h"
#include "common/common.h"

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_MASK (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_MASK (PIC2+1)

// PIC stuff
#define ICW1_ICW4 0x01 // ICW4 not needed
#define ICW1_SINGLE 0x02 // Single cascade mode
#define ICW1_INTERVAL4 0x04 // Call address interval 4
#define ICW1_LEVEL 0x08 // Level triggered
#define ICW1_INIT 0x10 // Initialization required

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM 0x10

extern "C" void setIDT(uint32_t);

__attribute__((interrupt)) void default_handler(intframe_t *iframe) {
    Terminal::writestring("Unregistered handler called!!!! Dumping frame\n");
    
    
    Terminal::writestring("Error code ");
    Terminal::writehex32(iframe->error_code);
    Terminal::newline();

    Terminal::writestring("EIP ");
    Terminal::writehex32(iframe->eip);
    Terminal::newline();
    
    Terminal::writestring("CS ");
    Terminal::writehex16(iframe->cs);
    Terminal::newline();
    
    Terminal::writestring("EFLAGS ");
    Terminal::writehex32(iframe->eflags);
    Terminal::newline();
}

__attribute__((interrupt)) void irq_master_handler(intframe_t *iframe) {
    uint16_t isr = PIC::GetISR();
    if(isr & 0x0001) {
        
    } else if(isr & 0x0002) {
        keyboard_irq();
    } else {
        Terminal::writestring("Unkown irq\n");
    }
    PIC::SendEOI(0);
}

__attribute__((interrupt)) void irq_slave_handler(intframe_t *iframe) {
    Terminal::writestring("PIC Slave called\n");
    PIC::SendEOI(8);
}

idt_entry_t IDT::idt_entries[256];
idt_ptr_t IDT::idt_ptr;

void IDT::Initialize() {
    idt_ptr.base = (uint32_t)&idt_entries;
    idt_ptr.limit = (uint16_t)(8 * 256) - 1;

    uint32_t address = (uint32_t)&default_handler;
    for(uint16_t i = 0; i < 256; i++) {
        SetEntry(i, address, 1, INTERRUPT_GATE);
    }

    setIDT((uint32_t)&idt_ptr);
}

void IDT::PIC_REMAP(uint16_t master_offset, uint16_t slave_offset) {
    uint8_t temp1, temp2;

    temp1 = IO::inb(PIC1_MASK);
    temp2 = IO::inb(PIC2_MASK);

    IO::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO::wait();
    IO::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO::wait();
    IO::outb(PIC1_MASK, master_offset);
    IO::wait();
    IO::outb(PIC2_MASK, slave_offset);
    IO::wait();
    IO::outb(PIC1_MASK, 4);
    IO::wait();
    IO::outb(PIC2_MASK, 2);
    IO::wait();

    IO::outb(PIC1_MASK, ICW4_8086);
    IO::wait();
    IO::outb(PIC2_MASK, ICW4_8086);
    IO::wait();

    IO::outb(PIC1_MASK, temp1);
    IO::outb(PIC2_MASK, temp2);
}

void IDT::SetDefaultIRQHandlers() {

    // Remap the PIC
    for(uint16_t irq = 0; irq < 16; irq++) {
        uint32_t address;
        if(irq < 8) {
            address = (uint32_t)&irq_master_handler;
        } else {
            address = (uint32_t)&irq_slave_handler;
        }
        SetEntry(irq+32, address, 1, INTERRUPT_GATE);
    }
}

void IDT::SetEntry(int32_t num, uint32_t offset, uint16_t gdt_entry, uint8_t type) {
    idt_entries[num].offset_low = (offset & 0xFFFF);
    idt_entries[num].offset_high = (offset >> 16 & 0xFFFF);
    idt_entries[num].selector = gdt_entry * 0x08; // Selector refers to which gdt entry and one gdt entry is 0x08 which means entry 1 is located at 0x08.
    idt_entries[num].type_attr = type;
}

__attribute__((interrupt)) void test_handler(intframe_t *frame) {
    Terminal::writestring("Interrupt 30 was succesfully called\n");
}

void IDT::TestIDT() {
    SetEntry(50, (uint32_t)&test_handler, 1, INTERRUPT_GATE);
    asm volatile("INT $0x32");
    asm volatile("INT $0x32");
    asm volatile("INT $0x32");
    asm volatile("INT $0x32");
    SetEntry(50, (uint32_t)&default_handler, 1, INTERRUPT_GATE);
}