#include "Scheduler.h"

extern "C" {
#include "cont.h"

void yield();
}

SchedulerClass Scheduler;

Task* SchedulerClass::first = NULL;
Task* SchedulerClass::current = NULL;
uint8_t SchedulerClass::nActiveGroupsIdx = 0;
uint8_t SchedulerClass::nActiveTasks = 0;
uint8_t SchedulerClass::scheduler_cycle_id = 0;
uint8_t SchedulerClass::scheduler_run_group_id = 0;

SchedulerClass::SchedulerClass() {}

void SchedulerClass::start(Task* task) {
  if (!first)
    first = current = task;
  else
    current = current->next = task;
}

void SchedulerClass::updateCurrentTask() {
  // This update runs before the call to shouldRun
  uint8_t task_run_group_id = current->run_group_id;

  // First, check the cycle_id
  if (current->current_cycle_id != scheduler_cycle_id) {
    // If the Task's cycle_id doesn't match; reset loop_complete and set the
    // current cycle_id
    if (task_run_group_id != 0xFF)
      current->loop_complete = false;
    current->current_cycle_id = scheduler_cycle_id;
  }

  // Next, update the Task's run_group_active flag
  // If this Task's run_group_id == 0xFF; run_group_active is true
  current->run_group_active = (task_run_group_id == 0xFF) ||
                              (task_run_group_id == scheduler_run_group_id);
}

void SchedulerClass::updateRunGroups() {
  // This is called after the Task had a chance to run; updateCurrentTask()
  // reset loop_complete and cycle_id if required and if part of the currently
  // active run_group, loop() could have set loop_complete

  // First, update the corresponding ActiveRunGroup bits
  uint8_t task_run_group_id = current->run_group_id;

  // Mark the group bit idx as an active group
  nActiveGroupsIdx |=
      (1 << task_run_group_id);  // This is cumulative, so any Task in the Group
                                 // will set the bit

  // If this Task is !loop_complete and also part of the current run_group,
  // increment nActiveTasks; this group_id isn't done yet
  if (!current->loop_complete && scheduler_run_group_id == task_run_group_id)
    nActiveTasks++;

  // Then, if a full loop through all the Tasks completed; update the run_group
  // state
  if (current->next == first) {
    // If there were no GroupActiveTasks found (all loop_completed == false) for
    // this group_id, move to the next group_id
    if (nActiveTasks == 0) {
      uint8_t nGroups = countRunGroups();

      scheduler_run_group_id = (scheduler_run_group_id + 1) % nGroups;
      if (scheduler_run_group_id == 0)
        scheduler_cycle_id++;  // If group_id wrapped back to 0; a cycle
                               // completed

      // nActiveGroupsIdx is a bit flag array for Group IDs with Tasks assigned
      // to them
      uint8_t nIncs = 0;  // Infinite Loop Protector
      while (/*nActiveGroupsIdx != 0 && */ (
                 nActiveGroupsIdx & (1 << scheduler_run_group_id)) == 0) {
        scheduler_run_group_id = (scheduler_run_group_id + 1) % nGroups;
        if (scheduler_run_group_id == 0)
          scheduler_cycle_id++;  // If group_id wraps back to 0; the cycle
                                 // completed
        nIncs++;
        if (nIncs >= nGroups - 1)
          break;  // If the counter has Incremented a full rotation; stop.
      }
    }
    nActiveTasks = 0;      // reset count of GroupActiveTasks
    nActiveGroupsIdx = 0;  // reset indeces of ActiveGroups
  }
}

void SchedulerClass::begin() {
  current = current->next = first;
  while (1) {
    updateCurrentTask();

    if (current->shouldRun())
      cont_run(&current->context, task_tramponline);

    updateRunGroups();

    ::yield();

    current = current->next;
  }
}

void SchedulerClass::delay(unsigned long ms) {
  current->delay(ms);
}

void SchedulerClass::yield() {
  current->yield();
}

void task_tramponline() {
  SchedulerClass::current->loopWrapper();
}
