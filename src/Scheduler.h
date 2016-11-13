#ifndef SCHEDULER_H
#define SCHEDULER_H

// #include <Arduino.h>
#include "Task.h"

extern "C" void loop();
extern void task_tramponline();

class SchedulerClass {
public:
    SchedulerClass();
	inline static uint8_t getCurrentRunGroupID() { return scheduler_run_group_id; }
	inline static uint8_t getCurrentCycleID() { return scheduler_cycle_id; }
    inline static uint8_t countRunGroups() { 
	    return ((sizeof nActiveGroupsIdx) * 8); // The count of group ids is the number of bits in nActiveGroupsIdx
    }
    static void updateRunGroups();
    static void updateCurrentTask();
	
    static void start(Task *task);

    static void begin();

private:
    friend void task_tramponline();

    // class MainTask : public Task {};

    static Task main;
    static Task *current;

    static uint8_t nActiveGroupsIdx;
    static uint8_t nActiveTasks;
    static uint8_t scheduler_cycle_id;
    static uint8_t scheduler_run_group_id;
};

extern SchedulerClass Scheduler;

#endif
