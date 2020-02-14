#Phyiscal Memory layout
0->1MiB Unused/Reserved  
1MiB->512MiB Reserved by kernel code and kernel structures for pages and such 
512MiB-> Dynamic memory managed by memory allocator  

Memory at 0 to 1MiB is unused because it can contain different memory mapped devices so to simplify the kernel unused space here is ignored.  
All memory from 1MiB->512MiB is reserved by the kernel for kernel code, paging, processes and dynamic kernel memory such as drivers.
Memory above 512MiB is avalable to userspace processes as virtualy mapped memory by a different memory allocator.

The linker script exposes to variables to detect the size of the kernel code called kernel_start and kernel_end.  
Note kernel_start will be 0x0 but in fact the actual kernel code starts att 0x100000

#Memory allocators.
The kernel contains 2 different memory allocators that manage seperat areas of memory.  
The first allocator works with kernel reserved dynamic memory located in the 1MiB->512MiB range.  
Note this allocator will start att a 4KiB aligned adress above 1MiB because the kernel code is loaded att 1MiB and takes up space that is defined att compile time and thus changes every time the code size increases and decreases.  

The second memory allocator is the memory allocator designed for userspace processes and will always start att 512MiB and go upp to the highest allowed memory address that has not been reserved by the BIOS.

#Paging and virtual addresses