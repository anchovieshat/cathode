set disassembly-flavor intel
set architecture i8086
target remote :1234
br *0x7c00
c
layout asm
