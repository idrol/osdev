#include "kernel/IO.h"

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

void IO::PICSendEOI(uint8_t irq) {
    if(irq >= 8) {
        outb(PIC2, PIC_EOI);
    }
    outb(PIC1, PIC_EOI);
}

void IO::Halt() {
    asm("HLT");
}

char* IO::itoa(uint32_t value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    //if ( value < 0 && base == 10 )
    //{
    //    *ptr++ = '-';
    //}
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

uint8_t IO::PIC_get_mask() {
    uint8_t mask = inb(PIC1_DATA);
    return mask;
}

void IO::PIC_set_mask(uint8_t irqLine) {
    uint16_t port;
    uint8_t val;

    if(irqLine < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqLine -= 8;
    }
    val = inb(port) | (1 << irqLine);
    outb(port, val);
}

void IO::PIC_clear_mask(uint8_t irqLine) {
    uint16_t port;
    uint8_t val;

    if(irqLine < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqLine -= 8;
    }
    val = inb(port) & ~(1 << irqLine);
    outb(port, val);
}

uint16_t IO::PIC_GET_IRQ_REG(int ocw3) {
    outb(PIC1, ocw3);
    outb(PIC2, ocw3);
    return (inb(PIC2) << 8) | inb(PIC1);
}

uint16_t IO::PIC_GET_IRR() {
    return PIC_GET_IRQ_REG(PIC_READ_IRR);
}

uint16_t IO::PIC_GET_ISR() {
    return PIC_GET_IRQ_REG(PIC_READ_ISR);
}