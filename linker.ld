/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)
 
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 0xC0100000; /* Link to 3.5GiB + 1MiB */
 
	kernel_start = .; /* Start address of the kernel will be 0x100000*/
	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */

	kernel_readonly_start = .; /* Boot assembly will create write protected pages around read only */
	.text ALIGN(4K) : AT(ADDR(.text)-0xC0000000)
	{
		*(.multiboot)
		*(.text)
	}
	
	/* Read-only data. */
	.rodata ALIGN(4K) : AT(ADDR(.rodata)-0xC0000000)
	{
		*(.rodata)
	}
	kernel_readonly_end = .;
 
	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data)-0xC0000000)
	{
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss  ALIGN(4K) : AT(ADDR(.bss)-0xC0000000)
	{
		*(COMMON)
		*(.bss)
	}
 
	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
	
	kernel_end = .;
}
