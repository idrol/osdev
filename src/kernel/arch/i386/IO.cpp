#include "kernel/arch/IO.h"

#define PIC1 0x20
#define PIC1_DATA PIC1 + 1
#define PIC2 0xA0
#define PIC2_DATA PIC2 + 1
#define PIC_EOI 0x20
#define PIC_READ_IRR 0x0a
#define PIC_READ_ISR 0x0b

void IO::outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1": : "a"(val), "Nd"(port));
}

uint8_t IO::inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void IO::wait() {
    asm volatile("jmp 1f\n\t"
                 "1:jmp 2f\n\t"
                 "2:");
}

void PIC::SendEOI(uint8_t irq) {
    if(irq >= 8) {
        IO::outb(PIC2, PIC_EOI);
    }
    IO::outb(PIC1, PIC_EOI);
}

void IO::Halt() {
    asm("HLT");
}

uint8_t PIC::GetMask() {
    uint8_t mask = IO::inb(PIC1_DATA);
    return mask;
}

void PIC::SetMask(uint8_t irqLine) {
    uint16_t port;
    uint8_t val;

    if(irqLine < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqLine -= 8;
    }
    val = IO::inb(port) | (1 << irqLine);
    IO::outb(port, val);
}

void PIC::ClearMask(uint8_t irqLine) {
    uint16_t port;
    uint8_t val;

    if(irqLine < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqLine -= 8;
    }
    val = IO::inb(port) & ~(1 << irqLine);
    IO::outb(port, val);
}

uint16_t PIC::GetIRQReg(int ocw3) {
    IO::outb(PIC1, ocw3);
    IO::outb(PIC2, ocw3);
    return (IO::inb(PIC2) << 8) | IO::inb(PIC1);
}

uint16_t PIC::GetIRR() {
    return GetIRQReg(PIC_READ_IRR);
}

uint16_t PIC::GetISR() {
    return GetIRQReg(PIC_READ_ISR);
}