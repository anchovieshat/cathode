OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i8086)

SECTIONS {
	. = 0x8000;
	.text16 : { *(.text16) }
	.text : { *(.text) }
}
