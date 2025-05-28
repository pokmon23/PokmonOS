#include "io_utils.h"

void outb(u16 port, u8 value) {
    asm volatile("outb %b0, %w1" : : "a"(value), "Nd"(port));


} 