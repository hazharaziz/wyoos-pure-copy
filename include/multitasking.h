#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace myos 
{
    struct CPUState
    {
        myos::common::uint32_t eax;
        myos::common::uint32_t ebx;
        myos::common::uint32_t ecx;
        myos::common::uint32_t edx;

        myos::common::uint32_t esi;
        myos::common::uint32_t edi;
        myos::common::uint32_t ebp;

        // myos::common::uint32_t gs;
        // myos::common::uint32_t fs;
        // myos::common::uint32_t es;
        // myos::common::uint32_t ds;

        myos::common::uint32_t error;

        myos::common::uint32_t eip;
        myos::common::uint32_t cs;
        myos::common::uint32_t eflags;
        myos::common::uint32_t esp; 
        myos::common::uint32_t ss;

    } __attribute__((packed));
    
    class Task
    {
        friend class TaskManager;
        private:
            common::uint8_t stack[4096]; // 4 KiB
            CPUState* cpuState;
        public:
            Task(GlobalDescriptorTable *gdt, void entrypoint());
            ~Task();
    };

    class TaskManager
    {
        private:
            Task* tasks[256];
            int numTasks;
            int currentTask;
        
        public:
            TaskManager();
            ~TaskManager();

            bool AddTask(Task* task);
            CPUState* Schedule(CPUState* cpuState);
    };

}


#endif