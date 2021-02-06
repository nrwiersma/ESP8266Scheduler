#ifndef TASK_H
#define TASK_H

#include <Arduino.h>

#include "Scheduler.h"

extern "C" {
#include "cont.h"
}
void task_tramponline();
class ITask {
 protected:
  virtual void setup() {}
  virtual void loop() {}
  virtual bool shouldRun() {
    unsigned long now = millis();
    return !delay_ms || now >= delay_time + delay_ms;
  }

  unsigned long delay_time;
  unsigned long delay_ms;

 private:
  friend class SchedulerClass;
  virtual void resume() {}
  ITask* next;
};

class Task : public ITask {
 public:
  Task() { cont_init(&context); }

 protected:
  void delay(unsigned long ms) {
    if (ms) {
      delay_time = millis();
      delay_ms = ms;
    }
    yield();
  }
  void yield() { cont_yield(&context); }

 private:
  friend void task_tramponline();
  cont_t context;
  virtual void resume() {
    if (shouldRun()) cont_run(&context, task_tramponline);
  }
  void loopWrapper() {
    setup();
    while (1) {
      loop();
      yield();
    };
  }
};

class LeanTask : public ITask {
 protected:
  void schedule(unsigned long ms) {
    if (ms) {
      delay_time = millis();
      delay_ms = ms;
    }
  }

 private:
  bool setup_done;
  virtual void resume() {
    if (!setup_done) {
      setup();
      setup_done = true;
    } else if (shouldRun())
      loop();
  }
};

#endif
