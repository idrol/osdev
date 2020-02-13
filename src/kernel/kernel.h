#pragma once
#include "grub/multiboot.h"

extern "C" void kernel_main(multiboot_info_t *mbd, uint32_t magic);

void kernel_panic();
