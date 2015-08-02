OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i8086)

SECTIONS {
	. = 0x7C5A;
	.text : { *(.text) }
}
