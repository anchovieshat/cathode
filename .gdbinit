set disassembly-flavor intel
#set architecture i386
set architecture i8086
target remote :1234
br *0x8000
c
layout asm
