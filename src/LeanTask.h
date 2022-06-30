#pragma once
#include <Arduino.h>

class LeanTask : public AbstractTask {
 public:
  LeanTask(bool _enabled = true, unsigned long _interval = 0) : AbstractTask(_enabled, _interval) {}

 protected:
  void resume() override {
    loopWrapper();
  }

  void yield() override {
    ::yield();
  }

 private:
  bool setup_done = false;
  void loopWrapper() override {
    if (!setup_done) {
      setup();
      setup_done = true;
    }

    loop();
    last_run = millis();
  }
};
