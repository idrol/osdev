#pragma once
#include <stddef.h>

enum vga_color {
  VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

enum video_type {
  VIDEO_TYPE_NONE = 0x00,
  VIDEO_TYPE_COLOR = 0x20,
  VIDEO_TYPE_MONOCHROME = 0x30,
};

class Terminal {
public:
  static void initialize();
  static void clear();
  static void writeline(const char* data);
  static void writestring(const char* data);
  static void write(const char* data, size_t size);

  // Write uint as integer
  static void writeint8(uint8_t num);
  static void writeint16(uint16_t num);
  static void writeint32(uint32_t num);
  static void writeint64(uint64_t num);

  // Write uint as hexadecimal
  static void writehex8(uint8_t num);
  static void writehex16(uint16_t num);
  static void writehex32(uint32_t num);
  static void writehex64(uint64_t num);


  static void newline();
  static void setColor(uint8_t color);
  static void enableCursor(uint8_t start, uint8_t end);
  static void disableCursor();
  static void updateCursor(size_t x, size_t y);

private:
  static uint16_t detectBiosAreaHardware();
  static video_type getVideoType();
  static void putchar(char c);
  static void putentryat(char c, uint8_t color, size_t x, size_t y);

  static uint16_t vga_entry(unsigned char uc, uint8_t color);
  static uint8_t vga_entry_color(enum vga_color, enum vga_color bg);

  static size_t row;
  static size_t column;
  static uint8_t color;
  static uint16_t* buffer;

  static const size_t VGA_WIDTH;
  static const size_t VGA_HEIGHT;
};
