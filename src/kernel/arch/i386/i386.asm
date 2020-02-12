section .text
    [GLOBAL setGDT]
    [GLOBAL setIDT]

setGDT:
    cli
    mov eax, [esp+4]
    lgdt [eax]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush
.flush
    ret

setIDT:
    mov eax, [esp+4]
    lidt [eax]
    ret