# Cathode

## Building Cathode on Linux

1. Install all dependencies:
	* `yasm`
	* `clang/gcc` (Requires edit to ninja file)
	* `ld`				(Requires edit to ninja file)
	* `objcopy`
	* `dd`
	* `parted`
	* `mtools`
	* `ninja`
	* `qemu`

3. Build and Install:
	```sh
	$ ninja
	$ qemu-system-x86_64 -m 512 -serial stdio build/boot.img
	```
