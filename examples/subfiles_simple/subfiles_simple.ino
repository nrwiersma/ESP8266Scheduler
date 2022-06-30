#include <Arduino.h>
#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>

#include "PrintTask.h"
#include "BlinkTask.h"
#include "MemTask.h"

PrintTask* tPrint;
BlinkTask* tBlink;
MemTask* tMem;

void setup() {
  Serial.begin(115200);
  Serial.println("");
  delay(1000);

  tPrint = new PrintTask(true);
  Scheduler.start(tPrint);

  tBlink = new BlinkTask(true, 1000);
  Scheduler.start(tBlink);

  tMem = new MemTask(true, 10000);
  Scheduler.start(tMem);

  Scheduler.begin();
}

void loop() {
  static unsigned long ts = 0;

  if ( millis() - ts > 1000 ) {
    Serial.println("Main loop");
    ts = millis();
  }
}
