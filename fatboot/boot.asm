bits 16

org 0x7C5A

FATCACHE: equ 0x7e00
LOADLOC: equ 0x8000

struc bpb
.jmp: resb 3
.oemid: resb 8
.sector_size: resw 1
.sectors_per_cluster: resb 1
.reserved_sectors: resw 1
.num_fats: resb 1
.num_rootdir: resw 1
.num_sectors: resw 1
.type: resb 1
.sectors_per_fat: resw 1
.sectors_per_track: resw 1
.num_heads: resw 1
.num_hidden_sectors: resd 1
.num_sectors_32: resd 1
; start FAT32-specific crap
.sectors_per_fat_32: resd 1
.flags: resw 1
.version: resw 1
.root_cluster: resd 1
.fsinfo_sector: resw 1
.backup_sector: resw 1
._reserved: resb 12
.drive_num: resb 1
._reserved2: resb 1
.signature: resb 1
.vol_id: resd 1
.vol_label: resb 11
.vol_sysid: resb 8
endstruc

struc fatde
.filename: resb 11
.attrs: resb 1
._reserved: resb 1
.ctime_s: resb 1
.ctime: resw 1
.cdate: resw 1
.adata: resw 1
.clust_hi: resw 1
.mtime: resw 1
.mdate: resw 1
.clust_lo: resw 1
.size: resd 1
endstruc

section .text align=1

jmp 0:start
start:
xor ax, ax
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov sp, 0x7C00
mov bp, sp

push dx ; save drive letter

stc
mov ah, 0x41
mov bx, 0x55aa
int 0x13

jc .baddisk
cmp bx, 0xaa55
jne .baddisk

xor ax, ax
mov al, [0x7C00+bpb.num_fats]
mul dword [0x7C00+bpb.sectors_per_fat_32]
add ax, word [0x7C00+bpb.reserved_sectors]
; ax now has first data sector
push ax

mov eax, [0x7C00+bpb.root_cluster]
mov ecx, eax
.nextsect:
sub eax, 2
mul byte [0x7C00+bpb.sectors_per_cluster]
add ax, [bp-4]

mov esi, eax ; FIXME from here
mov di, LOADLOC
call read_cluster

; Parse directory
mov bx, LOADLOC
.l
mov al, [bx]
xor di, di
mov di, bx
mov dx, bx
add bx, fatde_size
test al, al ; last entry
jz .more
cmp al, 0xE5 ; unused
je .l

mov si, filename
mov cx, 11
repe cmpsb
je .load_file
jmp .l

.more:

mov eax, ecx
call get_fat_cluster
cmp eax, 0x0FFFFFF8
jae .baddisk
jmp .nextsect

jmp .hlt

.baddisk:
mov cx, baddisk.size
mov bp, baddisk
call print
cli
.hlt:
hlt
jmp .hlt

.load_file:
mov bx, dx
mov ax, [bx+fatde.clust_hi]
shl eax, 16
or ax, [bx+fatde.clust_lo]

mov di, LOADLOC
.again:
mov esi, eax
sub esi, 2
mov ebx, eax
mov eax, esi
xor ecx, ecx
mov cl, [0x7C00+bpb.sectors_per_cluster]
mul ecx
add ax, [bp-4]
mov esi, eax
mov eax, ebx
call read_cluster
mov bx, ax
xor ax, ax
mov al, [0x7C00+bpb.sectors_per_cluster]
mul word [0x7C00+bpb.sector_size]
add di, ax
mov ax, bx
call get_fat_cluster
cmp eax, 0x0FFFFFF8
jae .done
jmp .again
.done:
jmp LOADLOC


; read one cluster from si into di, provided sl
read_cluster:
push ax
push bx

; patch DAP
xor ax, ax
mov al, [0x7C00+bpb.sectors_per_cluster]
mov bx, [dap.num_blocks]
;mov [dap.num_blocks], ax

call read

; unpatch DAP
;mov [dap.num_blocks], bx

pop bx
pop ax
ret

; read one sector from esi into edi, provided dl
read:
pushad
stc
mov ah, 0x42
mov dx, [bp-2]
mov [dap.lba], esi
mov [dap.dest], edi
mov si, dap
int 0x13
jc start.baddisk
test ah, ah
jnz start.baddisk
popad
ret

; returns entry for cluster ax, in eax
get_fat_cluster:
push si
push di
push cx

mov dx, 4 ; 32 bits
mul dx
mov cx, ax ; store this for later
mov si, [0x7C00+bpb.sector_size]
xor dx, dx ; high bits of dividend
div si
mov si, [0x7C00+bpb.reserved_sectors] ; first FAT sector
add ax, si
; ax now has sector number, so read it
mov si, ax
mov di, FATCACHE
call read

mov ax, cx ; now we need this (offset into FAT) to get the remainder
xor dx, dx
mov si, [0x7C00+bpb.sector_size]
div si
; now DX has remainder
mov si, dx
mov ax, si
;mov cx, 4
;mul cx
mov si, ax
mov eax, [FATCACHE+si]

; mask out "reserved" bits
and eax, 0x0FFFFFFF

pop cx
pop di
pop si
ret

print:
pushad
.l:
mov al, [bp]
inc bp
mov ah, 0x0E
xor bx, bx
int 0x10
loop .l
popad
ret

dap:
.size: db 0x10
._reserved: db 0
.num_blocks: dw 1
.dest: dd 0
.lba: dq 0

baddisk: db "Disk error"
.size: equ $-baddisk

filename: db "STAGE2  LDR"
