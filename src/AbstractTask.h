#pragma once
#include <Arduino.h>

class AbstractTask {
 public:
  AbstractTask(bool _enabled = true, unsigned long _interval = 0) {
    enabled = _enabled;
    interval = _interval;
  }

  bool isEnabled() {
    return enabled;
  }

  void enable() {
    enabled = true;
  }

  void disable() {
    enabled = false;
  }

  void setInterval(unsigned long val) {
    interval = val;
  }

  unsigned long getInterval() {
    return interval;
  }
  

 protected:
  virtual void resume() = 0;
  virtual void setup() {}
  virtual void loop() {}
  virtual void yield() = 0;

  virtual void delay(unsigned long ms) {
    if (ms) {
      delay_start = millis();
      delay_ms = ms;
    }

    yield();
  }

  inline bool isDelayed() {
    return delay_ms > 0 && (millis() - delay_start) < delay_ms;
  }

  virtual bool shouldRun() {
    if (!enabled || isDelayed() || !run_group_active) {
      return false;
    }

    if ( interval > 0 && (millis() - last_run) < interval ) {
      return false;
    }

    return !loop_complete;
  }


  bool enabled = true;
  unsigned long interval = 0;
  unsigned long last_run = 0;

  uint8_t current_cycle_id = 0;
  uint8_t run_group_id = 0xFF;
  bool run_group_active = false;
  bool loop_complete = false;

 private:
  friend class SchedulerClass;
  
  AbstractTask* next;
  unsigned long delay_start = 0;
  unsigned long delay_ms = 0;

  virtual void loopWrapper() = 0;
};
