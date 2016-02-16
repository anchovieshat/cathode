bits 16

section .text16
start:
	mov dx, [bp-2] ; keep drive letter around
	mov bp, msg
	mov cx, msg.size
	call print

	mov bp, disab
	mov cx, disab.size
	call check_a20
	test ax, ax
	jnz .s
	call print
	call die
.s:
	cli
.goto_prot
	lgdt [gdtr]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp 0x08:prot_start

die:
	cli
	hlt
	jmp die

print:
	push ax
	push bx
.l:
	mov al, [bp]
	inc bp
	mov ah, 0x0E
	xor bx, bx
	int 0x10
	loop .l
	pop bx
	pop ax
	ret

check_a20:
	push ds
	push es
	push ax
	push di
	push si

	cli ; so we don't damage something
	xor ax, ax
	mov es, ax ; normal es
	not ax
	mov ds, ax ; 0xFFFF ds

	mov di, 0x0500
	mov si, 0x0510
	mov al, [es:di] ; save originals
	push ax
	mov al, [ds:si]
	push ax

	mov byte [es:di], 0x00 ; write junk
	mov byte [ds:si], 0xFF ; here too
	cmp byte [es:di], 0xFF ; did we see it here?

	pop ax ; restore
	mov [ds:si], al
	pop ax
	mov [es:di], al

	mov ax, 0
	je .ret
	mov ax, 1
.ret:
	pop si
	pop di
	pop ax
	pop es
	pop ds
	sti
	ret

extern _kmain

section .text
bits 32
prot_start:
	mov eax, 0x10
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov ss, eax
	mov esp, 0x8000
	mov ebp, esp
	mov eax, edx
	movzx eax, dl
	push eax ; push drive letter, at [bp-4]

	call _kmain

	hlt
	jmp $

global idt_load
extern idtp

idt_load:
	lidt [idtp]
	sti
	ret

%macro isr 1
	global isr%1
	isr%1:
		cli
		push byte 0
		push byte %1
		jmp common_stub
%endmacro

%macro isr_err 1
	global isr%1
	isr%1
		cli
		push byte %1
		jmp common_stub
%endmacro

%macro irq 2
	global irq%1
	irq%1:
		cli
		push byte 0
		push byte %2
		jmp common_stub
%endmacro

isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isr_err 8
isr 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
isr 15
isr 16
isr 17
isr 18
isr 19
isr 20
isr 21
isr 22
isr 23
isr 24
isr 25
isr 26
isr 27
isr 28
isr 29
isr 30
isr 31
irq 0, 32
irq 1, 33
irq 2, 34
irq 3, 35
irq 4, 36
irq 5, 37
irq 6, 38
irq 7, 39
irq 8, 40
irq 9, 41
irq 10,	42
irq 11,	43
irq 12,	44
irq 13,	45
irq 14,	46
irq 15,	47

extern interrupt_handler

common_stub:
	pushad                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

	mov ax, ds               ; Lower 16-bits of eax = ds.
	push eax                 ; save the data segment descriptor

	mov ax, 0x10  ; load the kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call interrupt_handler

	pop eax        ; reload the original data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popad                     ; Pops edi,esi,ebp...
	add esp, 8     ; Cleans up the pushed error code and pushed ISR number
	sti
	iretd           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

section .rodata

msg: db "Booting stage2...",0xa,0xd
.size: equ $-msg

disab: db "A20 disabled...",0xa,0xd
.size: equ $-disab

global gdt
global gdtr

gdt:
.null:		  ; 0x000000000000000
	dw 0
	dw 0
	db 0
	db 0
	db 0
	db 0
.code32:		; 0xFFFF00009ACF
	dw 0xffff   ; Limit_low
	dw 0		; Base_low
	db 0		; Base_middle
	db 10011010b; Access Byte 0x9A
	db 11001111b; [Limit_high][Flags] 0xCF
	db 0		; Base_high
.data32:		; 0xFFFF0000092CF
	dw 0xffff
	dw 0
	db 0
	db 10010010b; 0x92
	db 11001111b; 0xCF
	db 0
.end:
gdtr:
	dw (gdt.end-gdt)-1
	dd gdt
