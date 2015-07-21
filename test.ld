OUTPUT_FORMAT(binary)
SECTIONS {
	. = 0x7C5A;
	.init . : { *(.init) }
	.text . : { *(.text) }
	.sig : AT(0x7DFE) {
		SHORT(0xaa55);
	}
}
