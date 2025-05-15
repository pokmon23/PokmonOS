#include "process_manager.h"
#include "../include/common/pokmonos.h"

using namespace PokmonOS;


ProcessManager::ProcessManager() :
    current_process_index(0),
    next_pid(1){
        for(u32 i = 0; i < MAX_PROCESSES; ++i){
            processes[i].pid = 0;
            processes[i].state = ProcessState::TERMINATED;
            processes[i].priority = 0;
            processes[i].stack_pointer = nullptr;
            processes[i].entry_point = nullptr;
        }
    }

u32 ProcessManager::createProcess(void* entry_point, u32 priority){
    ProcessControlBlock* new_process = findFreeProcessSlot();

    if(!new_process) return 0;

    new_process->pid = next_pid++;
    new_process->state = ProcessState::CREATED;
    new_process->priority = priority;
    new_process->entry_point = entry_point;


    new_process->stack_pointer = reinterpret_cast<void*>(
        reinterpret_cast<u32>(new_process->entry_point) - 4096
    );

    return new_process->pid;

}

ProcessControlBlock* ProcessManager::findFreeProcessSlot(){
    for(u32 i  =0; i < MAX_PROCESSES; ++i){
        if(processes[i].state == ProcessState::TERMINATED){
            return &processes[i];
        }
    }
    return nullptr;
}


void ProcessManager::scheduleNextProcess(){

    if(getTotalProcessCount() == 0) return;

    u32 start_index = current_process_index;

    do{
        current_process_index = (current_process_index + 1) % MAX_PROCESSES;

        if(processes[current_process_index].state == ProcessState::READY){
            processes[current_process_index].state = ProcessState::RUNNING;
            return;
        }
    }while(current_process_index != start_index);
}

void ProcessManager::blockProcess(u32 pid){
    for(u32 i = 0; i< MAX_PROCESSES; ++i){
        if(processes[i].pid == pid){
            processes[i].state = ProcessState::BLOCKED;
            return;
        }
    }
}

void ProcessManager::unblockProcess(u32 pid){
    for(u32 i = 0; i < MAX_PROCESSES; ++i){
        if(processes[i].pid == pid){
            processes[i].state = ProcessState::TERMINATED;
            processes[i].pid = 0;
            processes[i].stack_pointer = nullptr;
            processes[i].entry_point = nullptr;
            return;
        }
    }
}

u32 ProcessManager::getTotalProcessCount() const{
    u32 count = 0;
    
    for(u32 i = 0; i < MAX_PROCESSES; ++i){
        if(processes[i].state != ProcessState::TERMINATED){
            count++;
        }
    }
    return count;
}

u32 ProcessManager::getActiveProcessCount() const{
    u32 count = 0;
    for(u32 i = 0; i < MAX_PROCESSES; ++i){
        if(processes[i].state == ProcessState::RUNNING || processes[i].state == ProcessState::READY){
            count++;
        }
    }
    return count;
}


void ProcessManager::setPriority(u32 pid, u32 priority){
    for(u32 i = 0; i < MAX_PROCESSES; ++i){
        if(processes[i].pid == pid){
            processes[i].priority = priority;
            return;
        }
    }
}



void* ProcessManager::getProcessStackPointer(u32 pid) const{
    for(u32 i = 0; i < MAX_PROCESSES; ++i){
        if(processes[i].pid == pid){
            return processes[i].stack_pointer;
        }
    }
    return nullptr;
}
