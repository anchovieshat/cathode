bits 16

section .init
global _start
global do_read
extern main
extern die

jmp 0:_start
_start:

xor ax,ax
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov sp, 0x7C00
o32 call main

.hlt:
cli
hlt
jmp .hlt

do_read:
mov si, [esp+8]
mov ah, 0x42
mov dl, 0x80
stc
int 0x13
jnc .ret
jmp die
jmp _start.hlt
.ret:
ret

