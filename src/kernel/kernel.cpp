#include "interrupt_handler.h"
#include "../drivers/screen_driver.h"

extern "C" void kernel_main() {
    ScreenDriver screen;
    screen.write("PokmonOS: Własny system operacyjny!");

    InterruptHandler interrupts;
    interrupts.setupInterruptDescriptorTable();

    // reszta inicjalizacji...
}