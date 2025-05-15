#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "../include/common/pokmonos.h"

using namespace PokmonOS;



    class MemoryManager{
        private:
            static const u32 KERNEL_HEAP_START = 0x100000;
            static const u32 KERNEL_HEAP_SIZE = 0x100000;

            struct MemoryBlock{
                u32 address;
                u32 size;
                bool is_free;
            };

            MemoryBlock memory_blocks[256];
            u32 total_memory_blocks;
            u32 used_memory;

            void initializeMemoryBlocks();
            MemoryBlock* findFreeBlock(u32 size);
            void mergeFreeBlocks();

        public:
            MemoryManager();
            void initializeMemoryMap();
            void* allocate(u32 size);
            void free(void* pointer);


            u32 getTotalMemory() const;
            u32 getUsedMemory() const;
            u32 getFreeMemory() const;

            bool isValidPointer(void* pointer) const;
            
    };


#endif //MEMORY_MANAGER_H
