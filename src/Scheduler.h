#pragma once
#include "AbstractTask.h"

extern "C" void loop();

class SchedulerClass {
 public:
  SchedulerClass();
  inline static uint8_t getCurrentRunGroupID() {
    return scheduler_run_group_id;
  }
  inline static uint8_t getCurrentCycleID() { return scheduler_cycle_id; }
  inline static uint8_t countRunGroups() {
    return (
        (sizeof nActiveGroupsIdx) *
        8);  // The count of group ids is the number of bits in nActiveGroupsIdx
  }
  static void updateRunGroups();
  static void updateCurrentTask();

  static void start(AbstractTask* task);

  static void begin();
  static void delay(unsigned long ms);
  static void yield();
  static void trampoline();
  
 private:
  static AbstractTask* first;
  static AbstractTask* current;

  static uint8_t nActiveGroupsIdx;
  static uint8_t nActiveTasks;
  static uint8_t scheduler_cycle_id;
  static uint8_t scheduler_run_group_id;
  static bool mainLoopRunning;
};

extern SchedulerClass Scheduler;
