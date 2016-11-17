#include "Scheduler.h"
#include "RunGroups.h"

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
    SchedulerClass::start(task, 0xFF);
}

void SchedulerClass::start(Task *task, uint8_t run_group_id) {
    task->next = &main;
    task->prev = main.prev;
    task->run_group_info.id = run_group_id;

    main.prev->next = task;
    main.prev = task;
}

void SchedulerClass::begin() {
    // We start on main, so we need to do a full run through
    int tasksRun = 1;

    while (1) {
		if(canSchedule(current)) {
            cont_run(&current->context, task_tramponline);

            // If we have run a non-default task, increment the task count
            if (!RunGroups::defaultGroup(current->run_group_info)) {
                tasksRun++;
            }
        }

        // When we have hit main, we can reasonably check to update the run group id
        if (current == &main) {
            if (tasksRun == 0) {
                RunGroups::next();
            }

            tasksRun = 0;
        }

        yield();

        current = current->next;
    }
}

bool SchedulerClass::canSchedule(Task *task) {
    // If we are delayed, don't run
    if (task->delay_info.update(millis())) return false;

    // Otherwise run if the RunGroup allows it
    return RunGroups::current(task->run_group_info);
}

void task_tramponline() {
    SchedulerClass::current->loopWrapper();
}
