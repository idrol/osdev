.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */
 

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
 
 .section .bootstack, "aw", @nobits
.align 16
stack_bottom:
	.skip 16384 # 16 KiB
stack_top:

.section .bss, "aw", @nobits
.align 4096
boot_page_directory:
	.skip 4096

.section .text
.global _start
.type _start, @function
_start:
	movl $(stack_top - 0xC0000000), %esp // Stack top uses the virtual address but it is not setup yet
	// Push the multiboot params to the stack
	push %eax
	push %ebx

	//hlt
	
	movl $(boot_page_directory - 0xC0000000), %edi //Identity map 
	movl $0x83, (%edi)
	movl $0x0, %ecx

	_loop_empty_pages:
	inc %ecx
	add $0x4, %edi // Increment edi register by 4 bytes to get the next page directory
	movl $0x00000000, (%edi) // Mark this directory non present

	// Loop and clear the empty pages
	movl $(_loop_empty_pages), %eax
	cmp $767, %ecx
	jne _loop_empty_pages

	movl $0x0, %ecx

	_loop_virtual_pages:
	add $0x4, %edi // Increment edi register by 4 bytes to get the next page directory
	// Start att 0xC0000000 and add ecx * 0x00400000
	movl %ecx, %eax
	imul $0x00400000, %eax // Eax contains the physical address for this page
	or $0x83, %eax // Eax now should contain a valid 4MiB page directory
	movl %eax, (%edi)
	inc %ecx
	cmp $256, %ecx
	jne _loop_virtual_pages

	//Enable pse to use extended 4MiB pages
	movl %cr4, %eax
	or $0x10, %eax
	movl %eax, %cr4

	movl $(boot_page_directory - 0xC0000000), %edi
	//hlt
	movl %edi, %cr3
	movl $0, %edi // clear edi so we don't leave the pointer to the kernel page directory visible

	movl %cr0, %eax
	or $0x80010000, %eax
	//hlt
	movl %eax, %cr0

	lea 4f, %eax
	//hlt
	jmp %eax
	4:

	// We should now be higher half kernel

	// Restore multiboot variables
	pop %ebx
	pop %eax

	
	movl $0, boot_page_directory // Unmap the first 4MiB identity mapping
	
	// Flush the tlb
	movl %cr3, %edi
	movl %edi, %cr3

	mov $stack_top, %esp // Move that stack pointer to the mapped memory location
	
	add $0xC0000000, %ebx // Since the multiboot struct is att physical address we have to add the virtual address as offset

	push %eax
	push %ebx
	// Stack should be stack_top -8 so that the multiboot headers are passed
	call kernel_main
	pop %ebx
	pop %eax
 
	
	cli
1:	hlt
	jmp 1b
 
/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start
