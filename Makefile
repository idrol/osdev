src_c = $(shell find src/ -name "*.c")
src_cpp = $(shell find src/ -name "*.cpp")
src_asm = $(shell find src/ -name "*.asm")
boot_assembly = src/boot.s
boot_obj = build/src/boot.o

src = $(src_c) $(src_cpp)

_obj = $(src_c:.c=.o) $(src_cpp:.cpp=.o)
_obj_asm = $(src_asm:.asm=.o)

build_dir = build

obj = $(patsubst %,$(build_dir)/%,$(_obj))
obj_asm = $(patsubst %,$(build_dir)/%,$(_obj_asm))

target = i686-elf

CC = $(target)-g++
CXX = $(target)-g++
ASSEMBLER = nasm

CFLAGS = -std=c++17 -O0 -Wall -Wextra -fno-exceptions -fno-rtti -mgeneral-regs-only
CXXFLAGS = -std=c++17 -O0 -Wall -Wextra -fno-exceptions -fno-rtti -mgeneral-regs-only
ASMFLAGS = -f elf
QEMUFLAGS = -d guest_errors -d int -no-shutdown -no-reboot -monitor stdio -m 2G

INCLUDES = src/

create_build_dir:
	mkdir -p $(build_dir)
	cp -r src/ $(build_dir)/src/

$(src_c):
	$(CC) $(CFLAGS) -I $(INCLUDES) -c -o build/$(@:.c=.o) $@

$(src_cpp):
	$(CXX) $(CXXFLAGS) -I $(INCLUDES) -c -o build/$(@:.cpp=.o) $@

$(src_asm):
	$(ASSEMBLER) $(ASMFLAGS) $@ -o build/$(@:.asm=.o)

# Compile files
compile: create_build_dir $(src_asm) $(src_cpp) $(src_c)
	$(info $(src_asm))
	$(target)-as $(boot_assembly) -o $(boot_obj) 

# Build kernel image
link: compile
	$(target)-g++ -T linker.ld -o $(build_dir)/myos.bin -ffreestanding -O0 -nostdlib $(obj) $(obj_asm) $(boot_obj) -lgcc

# Build the grub iso
build: link
	mkdir -p $(build_dir)/isodir/boot/grub
	cp $(build_dir)/myos.bin $(build_dir)/isodir/boot/myos.bin
	cp grub.cfg $(build_dir)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(build_dir)/myos.iso $(build_dir)/isodir

run: build
	qemu-system-i386 -cdrom $(build_dir)/myos.iso $(QEMUFLAGS)

run_kernel: link
	qemu-system-i386 -kernel $(build_dir)/myos.bin $(QEMUFLAGS)

.PHONY: clean $(src_c) $(src_cpp) $(src_asm) compile
clean:
	rm -rf build/
