#include "Scheduler.h"

extern "C" {
#include "cont.h"

void yield();
}

SchedulerClass Scheduler;
ITask *SchedulerClass::last = NULL;
ITask *SchedulerClass::current = NULL;
SchedulerClass::SchedulerClass() {}

void SchedulerClass::start(ITask *task) {
  if (!last) {
    current = last = task;
  } else {
    last->next = task;
    last = task;
  }
}

void SchedulerClass::begin() {
  last->next = current;
  while (1) {
    current->resume();
    yield();
    current = current->next;
  }
}

void task_tramponline() { ((Task *)SchedulerClass::current)->loopWrapper(); }
