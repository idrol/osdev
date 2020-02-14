#pragma once
#include "common/stdint.h"

class IO {
public:
    static void outb(uint16_t port, uint8_t val);
    static uint8_t inb(uint16_t port);
    static void Halt();
    static void wait();
};

class PIC {
public:
    static void SendEOI(uint8_t irq);
    static uint8_t GetMask();
    static void SetMask(uint8_t irqLine);
    static void ClearMask(uint8_t irqLine);
    static uint16_t GetIRQReg(int ocw3);
    static uint16_t GetIRR();
    static uint16_t GetISR();
};