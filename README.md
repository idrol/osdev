#Phyiscal Memory layout
0->1MiB Unused/Reserved  
1MiB->4MiB Reserved by kernel code  
4MiB->2GiB Dynamic memory  

Note this memory layout does not represent the actual memory layout on the physical machine as the BIOS allocates different regions bellow 1MiB and att the end of the available memory. The memory manager does not currently handle BIOS memory allocated att the end of memory so make sure you allocated a bit of extra memory above 2GiB. If you don't do this at frist everything will be fine but will be undefined behaviour once the memory allocator starts allocating chunks that conflict with bios reserved areas att the end of memory.

The linker script exposes to variables to detect the size of the kernel code called kernel_start and kernel_end.  
Note kernel_start will be 0x0 but in fact the actual kernel code starts att 0x100000