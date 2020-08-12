#include "task.h"
#include "../types.h"
#include "debug.h"
#include "../page.h"

static Task *runningTask;
static Task mainTask; // this is the kernel
static Task otherTask;
 
static void otherMain() {
    BochsConsolePrintString("kernel-dispatch task\r\n");
    yield();
}
 
void initTasking() {
    // Get EFLAGS and CR3
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(mainTask.regs.cr3)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(mainTask.regs.eflags)::"%eax");
 
    *mainTask.name = "vitality-system";
    createTask(&otherTask, otherMain, mainTask.regs.eflags, (uint32_t*)mainTask.regs.cr3, "kernel-dispatch");
    mainTask.next = &otherTask;
    otherTask.next = &mainTask;
 
    runningTask = &mainTask;
}
 
void addTask(Task *task) {
    Task *nT = mainTask.next;
    Task *oT = 0;
    while(nT != 0) {
        oT = nT;
        nT = nT->next;
    }
    oT->next = task;
}

void createTask(Task *task, void (*main)(), uint32_t flags, uint32_t *pagedir, char name[16]) {
    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;
    task->regs.eflags = flags;
    task->regs.eip = (uint32_t) main;
    task->regs.cr3 = (uint32_t) pagedir;
    task->regs.esp = runningTask->regs.esp; // Not implemented here
    task->next = 0;
    *task->name = name;
}
 
void yield() {
    Task *last = runningTask;
    runningTask = runningTask->next;
    switchTask(&last->regs, &runningTask->regs);
}
