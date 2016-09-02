#ifndef SCHEDULER_H
#define SCHEDULER_H

// #include <Arduino.h>
#include "Task.h"

extern "C" void loop();
extern void task_tramponline();

class SchedulerClass {
public:
    SchedulerClass();

    static void start(Task *task);

    static void begin();

private:
    friend void task_tramponline();

    // class MainTask : public Task {};

    static Task main;
    static Task *current;
};

extern SchedulerClass Scheduler;

#endif
