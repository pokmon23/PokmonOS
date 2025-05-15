#ifndef KERNEL_H
#define KERNEL_H

#include "memory_manager.h"
#include "process_manager.h"
#include "interrupt_handler.h"
#include "../drivers/screen_driver.h"
#include "../include/common/pokmonos.h"
using namespace PokmonOS;

    class Kernel{
        private:
            MemoryManager memory_manager;
            ProcessManager process_manager;
            InterruptHandler interrupt_handler;
            ScreenDriver screen_driver;
        public:
            Kernel();
            void initialize();
            void run();

            void printKernelInfo();
            void handlePanic(const char* error_message);
    };

    extern "C" void kernel_main();


#endif //KERNEL_H