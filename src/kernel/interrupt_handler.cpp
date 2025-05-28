#include "interrupt_handler.h"
#include "../drivers/screen_driver.h"

using namespace PokmonOS;

InterruptHandlerFunction InterruptHandler::interrupt_handlers[256] = {nullptr};

InterruptHandler::InterruptHandler() {
    for (int i = 0; i < 256; ++i) {
        idt[i].selector = 0x08;
        idt[i].zero = 0;
        idt[i].type_attr = 0x8E;
        idt[i].offset_low = 0;
        idt[i].offset_high = 0;
    }
}

void InterruptHandler::setInterruptDescriptor(u8 vector, InterruptHandlerFunction handler, u16 selector, u8 type) {
    u32 address = reinterpret_cast<u32>(handler);
    idt[vector].offset_low = address & 0xFFFF;
    idt[vector].offset_high = (address >> 16) & 0xFFFF;
    idt[vector].selector = selector;
    idt[vector].type_attr = type;
}

extern "C" void divideByZeroHandler(ProcessorContext* context) {
    ScreenDriver screen;
    screen.write("Divide by Zero Error!");
}

void InterruptHandler::registerInterruptHandler(InterruptVector vector, InterruptHandlerFunction handler) {
    interrupt_handlers[static_cast<int>(vector)] = handler;
    setInterruptDescriptor(static_cast<u8>(vector), handler, 0x08, 0x8E);
}

void InterruptHandler::setupInterruptDescriptorTable() {
    // Przykładowe rejestrowanie obsługi przerwań
    registerInterruptHandler(InterruptVector::DIVIDE_BY_ZERO, divideByZeroHandler);
}