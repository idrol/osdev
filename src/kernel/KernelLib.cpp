#include "kernel/KernelLib.h"

size_t std::strlen(const char* str) {
  size_t len = 0;
  while(str[len]) {
    len++;
  }
  return len;
}

char std::keycodeToChar(uint8_t keycode) {
  if(keycode < 2 || keycode > 32) return ' ';
  const char *c = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  return c[keycode-2];
}