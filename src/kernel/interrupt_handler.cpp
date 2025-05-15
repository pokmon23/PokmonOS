#include "interrupt_handler.h"
#include "../include/common/pokmonos.h"
#include "../drivers/screen_driver.h"

using namespace PokmonOS;



void* InterruptHandler::interrupt_handlers[256] = {nullptr};

InterruptHandler::InterruptHandler(){
    for(int i = 0; i < 256; ++i){
        idt[i].selector = 0x08;
        idt[i].zero = 0;
        idt[i].type_attr = 0x8E;
        idt[i].offset_low = 0;
        idt[i].offset_high = 0;
    }
}


void InterruptHandler::setInterruptDescriptor(u8 vector, void* handler, u16 selector, u8 type){
    u32 address = reinterpret_cast<u32>(handler);
    idt[vector].offset_low = address & 0xFFFF;
    idt[vector].offset_high = (address >> 16) & 0xFFFF;
    idt[vector].selector = selector;
    idt[vector].type_attr = type;
}

void InterruptHandler::setupInterruptDescriptorTable(){
   registerInterruptHandler(
    InterruptVector::DIVIDE_BY_ZERO, 
    static_cast<void (*)(ProcessorContext*)>(
        [](ProcessorContext* context) {
            ScreenDriver screen;
            screen.write("Divide by Zero Error!");
        }
    )
);
};
