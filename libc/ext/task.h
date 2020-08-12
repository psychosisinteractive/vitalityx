/* Public Domain, Cooperative Multitasker */
#ifndef TASK_H
#define TASK_H
#include "../types.h"

void initTasking();
 
typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;
 
typedef struct Task {
    Registers regs;
    struct Task *next;
    char name[16];
} Task;
 
void initTasking();
void createTask(Task*, void(*)(), uint32_t, uint32_t*, char [16]);
void addTask(Task*);
void yield(); // Switch task frontend

extern void switchTask(Registers *, Registers *); // The function which actually switches

#endif