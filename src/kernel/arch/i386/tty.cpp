#include <stdbool.h>
#include "common/stdint.h"
#include "kernel/arch/tty.h"
#include "kernel/KernelLib.h"
#include "kernel/arch/IO.h"

#if !defined(__i386__)
  #error "Trying to compile ix86-elf code without ix86-elf compatible compiler"
#endif

size_t Terminal::row;
size_t Terminal::column;
uint8_t Terminal::color;
uint16_t* Terminal::buffer;

const size_t Terminal::VGA_WIDTH = 80;
const size_t Terminal::VGA_HEIGHT = 25;

void Terminal::initialize() {
  row = 0;
  column = 0;
  color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  buffer = (uint16_t*) 0xB8000; // Predefined location in memory for the vga buffer
  // The size of the buffer is determined but the vga mode.
  // Default is 80x25
  clear();
  enableCursor(0,1);
  updateCursor(0,1);
}

void Terminal::clear() {
  for(size_t y = 0; y < VGA_HEIGHT; y++) {
    for(size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      buffer[index] = vga_entry(' ', color);
    }
  }
}

void Terminal::setColor(uint8_t color) {
  Terminal::color = color;
}

void Terminal::putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  buffer[index] = vga_entry(c, color);
}

void Terminal::putchar(char c) {
  putentryat(c, color, column, row);
  if(++column == VGA_WIDTH) {
    column = 0;
    if(++row == VGA_HEIGHT) {
      row = 0;
    }
  }
}

void Terminal::write(const char* data, size_t size) {
  size_t column_start = column;
  for(size_t i = 0; i < size; i++) {
    if(data[i] == '\n') {
      for(size_t j = i; j < size; j++) {
        putentryat(' ', color, column_start + j, row);
      }
      column = 0;
      if(++row == VGA_HEIGHT) {
        row = 0;
        clear();
      }
    } else {
      putchar(data[i]);
    }
  }
  updateCursor(column, row+1);
}

void Terminal::writeline(const char* data) {
  writestring(data);
  newline();
}

void Terminal::writestring(const char* data) {
  write(data, std::strlen(data));
}

void Terminal::writeint8(uint8_t num) {
  int len = 4;
  char c[len];
  writestring(IO::itoa(num, c, 10));
}

void Terminal::writehex8(uint8_t num) {
  int len = 3;
  char c[len];
  writestring(IO::itoa(num, c, 16));
}

void Terminal::writeint16(uint16_t num) {
  int len = 6;
  char c[len];
  writestring(IO::itoa(num, c, 10));
}

void Terminal::writehex16(uint16_t num) {
  int len = 5;
  char c[len];
  writestring(IO::itoa(num, c, 16));
}

void Terminal::writeint32(uint32_t num) {
  int len = 11;
  char c[len];
  writestring(IO::itoa(num, c, 10));
}

void Terminal::writehex32(uint32_t num) {
  int len = 9;
  char c[len];
  write("0x", 2);
  writestring(IO::itoa(num, c, 16));
}

void Terminal::writeint64(uint64_t num) {
  int len = 20;
  char c[len];
  writestring(IO::itoa(num, c, 10));
}

void Terminal::writehex64(uint64_t num) {
  int len = 17;
  char c[len];
  writestring(IO::itoa(num, c, 16));
}

void Terminal::newline() {
  write("\n", 1);
}

uint8_t Terminal::vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

uint16_t Terminal::vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t) uc | (uint16_t) color << 8;
}

void Terminal::enableCursor(uint8_t start, uint8_t end) {
  IO::outb(0x3D4, 0x0A);
  IO::outb(0x3D5, (IO::inb(0x3D5) & 0xC0) | start);

  IO::outb(0x3D4, 0x0B);
  IO::outb(0x3D5, (IO::inb(0x3D5) & 0xE0) | end);
}

void Terminal::disableCursor() {
  IO::outb(0x3D4, 0x0A);
  IO::outb(0x3D5, 0x20);
}

void Terminal::updateCursor(size_t x, size_t y) {
  uint16_t pos = y * VGA_WIDTH + x;
  IO::outb(0x3D4, 0x0F);
  IO::outb(0x3D5, (uint8_t) (pos & 0xFF));
  IO::outb(0x3D4, 0x0E);
  IO::outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t Terminal::detectBiosAreaHardware() {
  const uint16_t *bda_detected_hardware_ptr = (const uint16_t*) 0x410;
  return *bda_detected_hardware_ptr;
}

video_type Terminal::getVideoType() {
  return (video_type) (detectBiosAreaHardware() & 0x30);
}