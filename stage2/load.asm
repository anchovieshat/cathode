bits 16

section .text16
global start
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
	lgdt [gdtr32]
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

	call check_cpuid
	call check_long_mode

	; Clear page tables
	mov edi, 0x1000
	mov cr3, edi
	xor eax, eax
	mov ecx, 4096
	rep stosd
	mov edi, cr3

	mov dword [edi], 0x2003 ; 0x0003 <- Present & Read-Write
	add edi, 0x1000
	mov dword [edi], 0x3003
	add edi, 0x1000
	mov dword [edi], 0x4003
	add edi, 0x1000

	mov ebx, 0x00000003
	mov ecx, 512

set_entry:
	mov dword [edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop set_entry

enable_PAE:
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

to_ia32e:
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	lgdt [gdtr64]
	jmp gdt64.code:long_start

	hlt
	jmp $

check_long_mode:
	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb no_long_mode

	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz no_long_mode

check_cpuid:
	pushfd
	pop eax

	mov ecx, eax

	xor eax, 1 << 21

	push eax
	popfd

	pushfd
	pop eax

	push ecx
	popfd
	xor eax, ecx
	jz no_cpuid
	ret

no_cpuid:
	jmp die32

no_long_mode:
	jmp die32

die32:
	cli
	hlt
	jmp die32

bits 64
long_start:
	cli
	mov ax, gdt64.data
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	call _kmain

	hlt

section .rodata

msg: db "Booting stage2...",0xa,0xd
.size: equ $ - msg

disab: db "A20 disabled...",0xa,0xd
.size: equ $ - disab

cpu_id: db "No CPUID...",0xa,0xd
.size: equ $ - cpu_id

global gdt32
global gdtr32
global gdt64
global gdtr64

gdt32:
.null:		  ; 0x000000000000000
	dw 0
	dw 0
	db 0
	db 0
	db 0
	db 0
.code:			; 0xFFFF00009ACF
	dw 0xffff   ; Limit_low
	dw 0		; Base_low
	db 0		; Base_middle
	db 10011010b; Access Byte 0x9A
	db 11001111b; [Limit_high][Flags] 0xCF
	db 0		; Base_high
.data:			; 0xFFFF0000092CF
	dw 0xffff
	dw 0
	db 0
	db 10010010b; 0x92
	db 11001111b; 0xCF
	db 0
.end:
gdtr32:
	dw (gdt32.end - gdt32) - 1
	dd gdt32

gdt64:							 ; Global Descriptor Table (64-bit).
.null: equ $ - gdt64			 ; The null descriptor.
	dw 0						 ; Limit (low).
	dw 0						 ; Base (low).
	db 0						 ; Base (middle)
	db 0						 ; Access.
	db 0						 ; Granularity.
	db 0						 ; Base (high).
.code: equ $ - gdt64			 ; The code descriptor.
	dw 0						 ; Limit (low).
	dw 0						 ; Base (low).
	db 0						 ; Base (middle)
	db 10011010b				 ; Access (exec/read).
	db 00100000b				 ; Granularity.
	db 0						 ; Base (high).
.data: equ $ - gdt64			 ; The data descriptor.
	dw 0						 ; Limit (low).
	dw 0						 ; Base (low).
	db 0						 ; Base (middle)
	db 10010010b				 ; Access (read/write).
	db 00000000b				 ; Granularity.
	db 0						 ; Base (high).
.end:
gdtr64:							 ; The GDT-pointer.
	dw (gdt64.end - gdt64) - 1	 ; Limit.
	dq gdt64					 ; Base.
