#include <Arduino.h>

class PrintTask : public Task {
public:
  PrintTask(bool _enabled = false, unsigned long _interval = 0) : Task(_enabled, _interval) {}

protected:
  void loop() {
    Serial.println("Print Loop Start");
    delay(5000);

    Serial.println("Print Loop End");
    delay(5000);
  }
};
