#include "task.h"
#include "../types.h"
#include "debug.h"
#include "../page.h"
#include "../system.h"
#include "../vitality/tty.h"

static Task *runningTask;
static Task mainTask; // this is the kernel
static Task otherTask;
 
static void otherMain() {
    tty_pputstring("Kernel dispatch\n");
    tty_pputstring("Waiting for something to happen...\n");
    yield();
}
 
void initTasking() {
    // Get EFLAGS and CR3
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(mainTask.regs.cr3)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(mainTask.regs.eflags)::"%eax");
 
    mainTask.flags = TASK_KERNEL | TASK_PROTECTED;
    mainTask.name = "vitality-system";
    mainTask.instantiated = true;
    createTask(&otherTask, otherMain, mainTask.regs.eflags, (uint32_t*)mainTask.regs.cr3, "kernel-dispatch",TASK_KERNEL | TASK_PROTECTED | TASK_CONSOLE);
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

void createTask(Task *task, void (*main)(), uint32_t flags, uint32_t *pagedir, char name[16], uint8_t tflags) {
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
    task->name = name;
    task->flags = tflags;
    task->instantiated = true;
    // set space for console
    memset(task->console,0,2000);
    tty_pputstring("New task ");
    tty_pputstring(name);
    tty_pputstring("\n");
}
 
void swapscreen(char *newconsole[]) {
    Task* ctask = getctask();
    memcpy(&ctask->console,0xb8000,2000);
    memcpy(0xb8000,newconsole,2000);
}

void yield() {
    Task *last = runningTask;
    Task *nextTask = runningTask->next;
    if(nextTask->instantiated) {
        uint8_t ntS = nextTask->flags;
        while(ntS & TASK_SKIP | ntS & TASK_KILLED) {
            nextTask = nextTask->next;
            uint8_t ntS = nextTask->flags;
        }
        tty_pputstring("Swapping to ");
        tty_pputstring(nextTask->name);
        tty_pputstring("\n");
        if(nextTask->flags & TASK_CONSOLE) {
            BochsConsolePrintString("Swapping consoles\n");
            swapscreen(&nextTask->console);
        }
        bochs_bkpt();
        switchTask(&last->regs, &nextTask->regs);
        void (*prog) ();
        prog = nextTask->regs.eip;
        prog();
    }
}

struct Task* getctask() {
    return runningTask;
}