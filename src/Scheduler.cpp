#include "Scheduler.h"

extern "C" {
    #include "cont.h"

    void yield();
}

SchedulerClass Scheduler;

Task SchedulerClass::main;
Task *SchedulerClass::current = &SchedulerClass::main;

SchedulerClass::SchedulerClass() {
    main.next = &main;
    main.prev = &main;
}

void SchedulerClass::start(Task *task) {
    task->next = &main;
    task->prev = main.prev;

    main.prev->next = task;
    main.prev = task;
}

void SchedulerClass::begin() {
    while (1) {
        if (current->shouldRun())
            cont_run(&current->context, task_tramponline);

        yield();

        current = current->next;
    }
}

void task_tramponline() {
    SchedulerClass::current->loopWrapper();
}
