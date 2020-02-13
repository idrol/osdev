section .text
    [GLOBAL setGDT]
    [GLOBAL setIDT]
    [GLOBAL loadPageDirectory]
    [GLOBAL enablePaging]

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

loadPageDirectory:
    push ebp
    mov ebp, esp
    mov eax, [esp+8]
    mov cr3, eax
    mov esp, ebp
    pop ebp
    ret

enablePaging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 80000000h
    mov cr0, eax
    mov esp, ebp
    pop ebp
    ret