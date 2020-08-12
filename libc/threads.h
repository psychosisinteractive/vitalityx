#ifndef THREADS_H
#define THREADS_H
#include "types.h"
#include "string.h"

///
/// Thread Priority States
///
enum tps
 {
    ///
    /// Lowest state, 
    ///
    TPSLow,
    ///
    /// Medium, takes prioity over below tasks.
    ///
    TPSMedium,
    ///
    /// High, takes priority over below tasks.
    ///
    TPSHigh,
    ///
    /// System level, takes priority over ALL tasks.
    ///
    TPSSystem,
} typedef tps_t;

///
/// Task States
///
enum tst 
{
    ///
    /// Waiting for the task to start
    ///
    TSTWaiting,
    ///
    /// Task is running
    ///
    TSTRunning,
    ///
    /// Task has finished and has gracefully exited
    ///
    TSTStopped,
    ///
    /// The Task has encountered an fatal error and stopped
    ///
    TSTErrored,
    ///
    /// System task, shouldnt be run in userspace
    ///
    TSTSystem,
} typedef tst_t;

///
/// Thread Descriptor Table
///
struct tdt 
{
    ///
    /// Address of the Task
    ///
    addr_t task_address;
    ///
    /// Name of the Task
    ///
    string_t task_name;
    ///
    /// Task Priority
    ///
    tps_t priority;
    ///
    /// Task State
    ///
    tst_t state;
};

#endif