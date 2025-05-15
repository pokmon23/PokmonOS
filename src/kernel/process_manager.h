#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "../include/common/pokmonos.h"

using namespace PokmonOS;

struct Registers{
        u32 eax, ebx, ecx, edx;
        u32 esi, edi, esp, ebp;
        u32 eip, eflags;
    };
enum class ProcessState{
    CREATED,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};
struct ProcessControlBlock{
            u32 pid;
            ProcessState state;
            Registers registers;
            u32 priority;
            void* stack_pointer;
            void* entry_point;
        };
class ProcessManager{
    private:
        static const u32 MAX_PROCESSES = 64;

        


        ProcessControlBlock processes[MAX_PROCESSES];
        u32 current_process_index;
        u32 next_pid;



        ProcessControlBlock* findFreeProcessSlot();
        void switchContext(ProcessControlBlock* from, ProcessControlBlock* to);

    public:
        ProcessManager();
        u32 createProcess(void* entry_point, u32 priority = 1);

        void scheduleNextProcess();
        void blockProcess(u32 pid);
        void unblockProcess(u32 pid);
        void terminateProcess(u32 pid);

        u32 getTotalProcessCount() const;
        u32 getActiveProcessCount() const;
        ProcessState getProcessState(u32 pid) const;

        void setPriority(u32 pid, u32 priority);
        void* getProcessStackPointer(u32 pid) const;
        
};


#endif  // PROCESS_MANAGER_H