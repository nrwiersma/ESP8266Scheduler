#include <Arduino.h>

class BlinkTask : public LeanTask {
public:
  BlinkTask(bool _enabled = false, unsigned long _interval = 0) : LeanTask(_enabled, _interval) {}

protected:
  void setup() {
    state = HIGH;

    pinMode(2, OUTPUT);
    pinMode(2, state);
  }

  void loop() {
    state = state == HIGH ? LOW : HIGH;
    pinMode(2, state);
  }

private:
  uint8_t state;
};
