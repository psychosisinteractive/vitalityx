/* Public Domain, Cooperative Multitasker */
#ifndef TASK_H
#define TASK_H
#include "../types.h"

#define TASK_SKIP 8
#define TASK_KILLED 4
#define TASK_PROTECTED 2
#define TASK_KERNEL 1

void initTasking();
 
typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;
 
typedef struct Task {
    Registers regs;
    struct Task *next;
    char *name;
    uint8_t flags;
} Task;
 
void initTasking();
void createTask(Task*, void(*)(), uint32_t, uint32_t*, char [16]);
void addTask(Task*);
void yield(); // Switch task frontend
struct Task* getctask();

extern void switchTask(Registers *, Registers *); // The function which actually switches

#endif