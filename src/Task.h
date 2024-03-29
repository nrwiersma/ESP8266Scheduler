#pragma once
#include <Arduino.h>

extern "C" {
#include "cont.h"
}

class Task : public AbstractTask {
 public:
  Task(bool _enabled = true, unsigned long _interval = 0) : AbstractTask(_enabled, _interval) {
    cont_init(&context);
  }

 protected:
  void resume() override {
    cont_run(&context, Scheduler.trampoline);
  }

  void yield() override {
    ::yield();
    cont_suspend(&context);
  }

 private:
  friend class SchedulerClass;

  cont_t context;
  bool setup_done = false;

  void loopWrapper() override {
    if (!setup_done) {
      setup();
      setup_done = true;
    }

    while (true) {
      loop();
      last_run = millis();
      yield();
    }
  }
};
