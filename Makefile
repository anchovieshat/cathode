.PHONY: all
.SUFFIXES:

all: boot.img

boot.tmpl:
	@echo HDIUTIL $@
	@hdiutil create -ov -fs "MS-DOS FAT32" -layout NONE -megabytes 40 $@
	@mv $@.dmg $@

boot.img: boot.tmpl boot.bin
	@cp $< $@
	@echo DD $@
	@dd if=boot.bin of=$@ conv=notrunc bs=1 count=420 seek=90

#boot.bin: boot.o
#	@echo LD $@
#	@x86_64-unknown-linux-ld -Tboot.ld -o $@ $^

boot.bin: boot.asm
	@echo YASM $@
	@yasm -fbin -o $@ $<

