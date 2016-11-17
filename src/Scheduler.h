#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"

extern "C" void loop();
extern void task_tramponline();

class SchedulerClass {
public:
    SchedulerClass();

    static void start(Task *task);
    static void start(Task *task, uint8_t run_group_id);

    static void begin();

private:
    friend void task_tramponline();

    static Task main;
    static Task *current;

    static bool canSchedule(Task *task);
};

extern SchedulerClass Scheduler;

#endif
