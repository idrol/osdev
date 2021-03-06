#include "kernel/arch/Exceptions.h"
#include "kernel/arch/IDT.h"
#include "kernel/arch/tty.h"
#include "common/debug_config.h"

__attribute__((interrupt)) void divideByZero(intframe_t *iframe) {
    Terminal::writeline("Divide by zero!!!");
}

__attribute__((interrupt)) void debug(intframe_t *interrupt_frame) {
    Terminal::writeline("Debug exception called");
}

__attribute__((interrupt)) void nmi(intframe_t *interrupt_frame) {
    Terminal::writeline("Non maskable interrupt called");
}

__attribute__((interrupt)) void breakpoint(intframe_t *interrupt_frame) {
    Terminal::writeline("Breakpoint called");
}

__attribute__((interrupt)) void overflow(intframe_t *interrupt_frame) {
    Terminal::writeline("Overflow exception called");
}

__attribute__((interrupt)) void bound_range(intframe_t *interrupt_frame) {
    Terminal::writeline("Bound range exceeded");
}

__attribute__((interrupt)) void invalid_opcode(intframe_t *interrupt_frame) {
    Terminal::writeline("Invalid opcode called");
}

__attribute__((interrupt)) void device_not_available(intframe_t *interrupt_frame) {
    Terminal::writeline("Device not available called");
}

__attribute__((interrupt)) void double_fault(intframe_t *interrupt_frame, uword_t error_code) {
    Terminal::writeline("Double fault!!!!!");
}

__attribute__((interrupt)) void coproccessor_seg_overrun(intframe_t *interrupt_frame) {
    // Should never be called. (Existed when fpu was a seperate processor)
    Terminal::writeline("Co-processor segment overrun");
}

__attribute__((interrupt)) void invalid_tss(intframe_t *interrupt_frame, uword_t error_code) {
    Terminal::writeline("Invalid TSS");
}

__attribute__((interrupt)) void segment_not_present(intframe_t *interrupt_frame, uword_t error_code) {
    Terminal::writeline("Segment not present");
}

__attribute__((interrupt)) void stack_segment_fault(intframe_t *interrupt_frame, uword_t error_code) {
    Terminal::writeline("Stack segment fault");
}

__attribute__((interrupt)) void gpf(intframe_t *interrupt_frame, uword_t error_code) {
    Terminal::writeline("General protection fault");
    asm volatile("hlt");
}

__attribute__((interrupt)) void page_fault(intframe_t *interrupt_frame, uword_t error_code) {
    // The address that caused this exception is stored in CR2 but we can only access it from assembly
    bool present = error_code & 0x1; // If set the fault occured due to page protection. If not set the fault was cause by non present page
    bool isWriteError = error_code & 0x1 << 1; // Indicates whethe this fault was caused by a write or read operation
    // If i understood the documentation the flags bellow should only be accessed if you are sure about certain states in the cpu and present = true
    bool isUser = error_code & 0x1 << 2; // The access level the code that caused this error had. This doesn't mean that it is a privilige error if this is true.
    bool reserved_write = error_code & 0x1 << 3;
    bool instruction_fetch = error_code & 0x1 << 4;
    
    #if DEBUG_VERBOSE
    Terminal::writestring("Page Fault: ");
    if(present) {
        // Page protection error
        // TODO add error checking for bit 2..4
        if(isWriteError) {
            Terminal::writeline("Tried to write to protected page!");
        } else {
            Terminal::writeline("Tried to read from protected page!");
        }
    } else {
        // Page is not present error
        if(isWriteError) {
            Terminal::writeline("Tried to write to non existant page!");
        } else {
            Terminal::writeline("Tried to read from non existant page!");
        }
    }
#else
    Terminal::writeline("Page fault!");
#endif

    #if DEBUG_HLT_PAGE_FAULT
    asm volatile("hlt");
    #endif
}

__attribute__((interrupt)) void floating_point_exception(intframe_t *interrupt_frame) {
    Terminal::writeline("x87 floating point exception");
}

__attribute__((interrupt)) void alignment_check(intframe_t *interrupt_frame, uword_t error_code) {
    Terminal::writeline("Alignment check exception");
}

__attribute__((interrupt)) void machine_check(intframe_t *interrupt_frame) {
    Terminal::writeline("Machine check exception");
}

__attribute__((interrupt)) void simd_fp_exception(intframe_t *interrupt_frame) {
    Terminal::writeline("SIMD floating point exception");
}

__attribute__((interrupt)) void virt_exception(intframe_t *interrupt_frame) {
    Terminal::writeline("Virtualization exception");
}

__attribute__((interrupt)) void security_exception(intframe_t *interrupt_frame, uword_t error_code) {
    Terminal::writeline("Security exception");
}

void Exceptions::Initialize() {
    SetException(0, (uint32_t)&divideByZero);
    SetException(1, (uint32_t)&debug);
    SetException(2, (uint32_t)&nmi);
    SetException(3, (uint32_t)&breakpoint);
    SetException(4, (uint32_t)&overflow);
    SetException(5, (uint32_t)&bound_range);
    SetException(6, (uint32_t)&invalid_opcode);
    SetException(7, (uint32_t)&device_not_available);
    SetException(8, (uint32_t)&double_fault);
    // 9 Used to be Coprocessor segment overrun. But has not been used sinze fpus was built into cpus.
    SetException(10, (uint32_t)&invalid_tss);
    SetException(11, (uint32_t)&segment_not_present);
    SetException(12, (uint32_t)&stack_segment_fault);
    SetException(13, (uint32_t)&gpf);
    SetException(14, (uint32_t)&page_fault);
    // 15 reserved
    SetException(16, (uint32_t)&floating_point_exception);
    SetException(17, (uint32_t)&alignment_check);
    SetException(18, (uint32_t)&machine_check);
    SetException(19, (uint32_t)&simd_fp_exception);
    SetException(20, (uint32_t)&virt_exception);
    // 21-29 reserved
    SetException(30, (uint32_t)&security_exception);
    // 31 reserved
}

void Exceptions::SetException(uint8_t exceptionNum, uint32_t addr) {
    IDT::SetEntry(exceptionNum, addr, 1, INTERRUPT_GATE);
}