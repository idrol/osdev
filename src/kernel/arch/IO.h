#pragma once
#include "common/stdint.h"

class IO {
public:
    static void outb(uint16_t port, uint8_t val);
    static uint8_t inb(uint16_t port);
    static void PICSendEOI(uint8_t urq);
    static void Halt();
    static char* itoa( uint32_t value, char * str, int base );
    static void wait();
    static uint8_t PIC_get_mask();
    static void PIC_set_mask(uint8_t irqLine);
    static void PIC_clear_mask(uint8_t irqLine);
    static uint16_t PIC_GET_IRQ_REG(int ocw3);
    static uint16_t PIC_GET_IRR();
    static uint16_t PIC_GET_ISR();
};