#include <Arduino.h>
#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>

#define TASK_TYPE Task
//#define TASK_TYPE LeanTask


class Blank1Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 1");
    delay(1010);
  }
} blank1_task;

class Blank2Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 2");
    delay(1020);
  }
} blank2_task;

class Blank3Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 3");
    delay(1030);
  }
} blank3_task;

class Blank4Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 4");
    delay(1040);
  }
} blank4_task;

class Blank5Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 5");
    delay(1050);
  }
} blank5_task;

class Blank6Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 6");
    delay(1060);
  }
} blank6_task;


class Blank7Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 7");
    delay(1070);
  }
} blank7_task;

class Blank8Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 8");
    delay(1080);
  }
} blank8_task;

class Blank9Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 9");
    delay(1090);
  }
} blank9_task;

class Blank10Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 10");
    delay(1100);
  }
} blank10_task;

class Blank11Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 11");
    delay(1110);
  }
} blank11_task;

class Blank12Task : public TASK_TYPE {
protected:
  void loop() {
    Serial.println("Blank 12");
    delay(1120);
  }
} blank12_task;


void setup() {
  Serial.begin(115200);
  Serial.println("");
  delay(1000);

  Scheduler.start(&blank1_task);
  Scheduler.start(&blank2_task);
  Scheduler.start(&blank3_task);
  Scheduler.start(&blank4_task);
  Scheduler.start(&blank5_task);
  Scheduler.start(&blank6_task);
  Scheduler.start(&blank7_task);
  Scheduler.start(&blank8_task);
  Scheduler.start(&blank9_task);
  Scheduler.start(&blank10_task);
  Scheduler.start(&blank11_task);
  Scheduler.start(&blank12_task);

  Scheduler.begin();
}

void loop() {
  static unsigned long ts = 0;
  if (millis() - ts > 10000) {
    Serial.print("Free Heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");

    ts = millis();
  }
}
