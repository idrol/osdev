src_c = $(shell find src/ -name "*.c")
src_cpp = $(shell find src/ -name "*.cpp")
src_asm = $(shell find src/ -name "*.asm")

src = $(src_c) $(src_cpp)
_obj = $(src_c:.c=.o) $(src_cpp:.cpp=.o)
_obj_asm = $(src_asm:.asm=.o)

build_dir = build

obj = $(patsubst %,$(build_dir)/%,$(_obj))
obj_asm = $(patsubst %,$(build_dir)/%,$(_obj_asm))

target = i686-elf

# Compile files
compile:
	$(info $(obj))
	mkdir -p $(build_dir)
	cp -r src/ $(build_dir)/src/
	$(target)-as $(src_asm) -o $(obj_asm)
	$(target)-g++ -c $(src) -o $(obj) -std=gnu99 -ffreestanding -O0 -Wall -Wextra -fno-exceptions -fno-rtti

# Build kernel image
link: compile
	$(target)-g++ -T linker.ld -o $(build_dir)/myos.bin -ffreestanding -O0 -nostdlib $(obj) $(obj_asm) -lgcc

# Build the grub iso
build: link
	mkdir -p $(build_dir)/isodir/boot/grub
	cp $(build_dir)/myos.bin $(build_dir)/isodir/boot/myos.bin
	cp grub.cfg $(build_dir)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(build_dir)/myos.iso $(build_dir)/isodir

run: build
	qemu-system-i386 -cdrom $(build_dir)/myos.iso

.PHONY: clean
clean:
	rm -rf build/
