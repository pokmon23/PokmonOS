#include "memory_manager.h"
#include "../include/common/pokmonos.h"

using namespace PokmonOS;

MemoryManager::MemoryManager() :
    total_memory_blocks(0),
    used_memory(0){
        initializeMemoryBlocks();
    }

    void MemoryManager::initializeMemoryBlocks(){
        for(u32 i = 0; i < 256; ++i){
            memory_blocks[i].address = 0;
            memory_blocks[i].size = 0;
            memory_blocks[i].is_free = true;
        }


        memory_blocks[0].address = KERNEL_HEAP_START;
        memory_blocks[0].size = KERNEL_HEAP_SIZE;
        memory_blocks[0].is_free = true;
        total_memory_blocks = 1;
    }

    void MemoryManager::initializeMemoryMap(){
        used_memory = 0;
    }

    void* MemoryManager::allocate(u32 size){
        for(u32 i = 0; i< total_memory_blocks; ++i){
            if(memory_blocks[i].is_free && memory_blocks[i].size >= size){
                memory_blocks[i].is_free = false;
                used_memory += size;

                if(memory_blocks[i].size > size + 32){
                    if(total_memory_blocks < 256){
                        memory_blocks[total_memory_blocks].address = memory_blocks[i].address + size;
                        memory_blocks[total_memory_blocks].size -= size;
                        memory_blocks[total_memory_blocks].is_free = true;
                        total_memory_blocks++;
                    }

                    memory_blocks[i].size = size;
                }
                return reinterpret_cast<void*>(memory_blocks[i].address);
            }
        }

        return nullptr;
    }


    void MemoryManager::free(void* pointer){
        if(!isValidPointer(pointer)) return;

        for(u32 i = 0; i< total_memory_blocks; ++i){
            if(reinterpret_cast<void*>(memory_blocks[i].address) == pointer){
                memory_blocks[i].is_free = true;
                used_memory -= memory_blocks[i].size;

                mergeFreeBlocks();
                return;
            }
        }
    }


    void MemoryManager::mergeFreeBlocks(){
        for(u32 i = 0; i < total_memory_blocks - 1; ++i){
            if(memory_blocks[i].is_free && memory_blocks[i+1].is_free){
                memory_blocks[i].size += memory_blocks[i+1].size;


                for(u32 j = i + 1; j < total_memory_blocks -1; ++j){
                    memory_blocks[j] = memory_blocks[j + 1];
                }

                total_memory_blocks--;
                i--;
            }
        }
    }


    bool MemoryManager::isValidPointer(void* pointer) const{
        u32 address = reinterpret_cast<u32>(pointer);

        return (address >= KERNEL_HEAP_START && address < (KERNEL_HEAP_START + KERNEL_HEAP_SIZE));
    }


    u32 MemoryManager::getTotalMemory() const{
        return KERNEL_HEAP_SIZE;
    }
    u32 MemoryManager::getUsedMemory() const{
        return used_memory;
    }

    u32 MemoryManager::getFreeMemory() const{
        return KERNEL_HEAP_SIZE - used_memory;
    }