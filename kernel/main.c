#include "serial.h"

void _kmain() {
  init_serial();
  sprint("----------------------\n");
  sprint("Hello from the kernel!\n");
}
