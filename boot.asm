bits 16

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

section .text

start:
cli
xor ax, ax
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
mov sp, 0x7C00

mov bp, 0x7C00+bpb.vol_sysid
mov cx, 8
call print

cli
.hlt:
hlt
jmp .hlt

print:
pushad
.l:
mov ax, [bp]
inc bp
mov ah, 0x0E
xor bx, bx
int 0x10
loop .l
popad
ret
