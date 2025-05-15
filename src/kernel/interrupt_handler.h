#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#include "../include/common/pokmonos.h"

using namespace PokmonOS;

enum class InterruptVector{
        DIVIDE_BY_ZERO = 0,
        DEBUG = 1,
        NMI = 2,
        BREAKPOINT = 3,
        OVERFLOW = 4,
        BOUNDS = 5,
        INVALID_OPCODE = 6,
        DEVICE_NOT_AVAILABLE = 7,
        DEVICE_FAULT = 8,
        COPROCESSOR_SEGMENT_OVERRUN = 9,
        INVALID_TSS = 10,
        SEGMENT_NOT_PRESENT = 11, 
        STACK_SEGMENT_FAULT = 12,
        GENERAL_PROTECTION = 13,
        PAGE_FAULT = 14,
        FLOATING_POINT_ERROR = 16,
        ALIGNMENT_CHECK = 17,
        MACHINE_CHECK = 18,
        SYSCALL = 0x80

    };

    struct InterruptDescriptor{
        u16 offset_low;
        u16 selector;
        u8 zero;
        u8 type_attr;
        u16 offset_high;
        }
        #if defined(__GNUC__) || defined(__clang__)
        __attribute__((packed))
        #elif defined(_MSC_VER)
        __pragma(pack(push,1))
        #endif
        ;

    struct ProcessorContext{
        u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
        u32 interrupt_number;
        u32 error_code;
        u32 eip, csm, eflags, user_esp, ss;
    };

    class InterruptHandler{
    private:
        InterruptDescriptor idt[256];

       static void* interrupt_handlers[256];

        void setInterruptDescriptor(u8 vector, void* handler, u16 selector, u8 type);
        void remapPIC();
    public:
        InterruptHandler();

        void setupInterruptDescriptorTable();

        void registerInterruptHandler(InterruptVector vector, void* handler);

        void handlePageFault(ProcessorContext* context);
        void handleGeneralProtectionFault(ProcessorContext* context);
        void handleSystemCall(ProcessorContext* context);


        void enableInterrupts();
        void disableInterrupts();

        void triggerInterrupt(InterruptVector vector);
    };
    
   

#endif // INTERRUPT_HANDLER_H