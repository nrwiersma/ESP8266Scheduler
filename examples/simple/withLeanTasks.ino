#include <Arduino.h>
#include <Scheduler.h>

class PrintTask : public Task {
protected:
    void loop()  {
        Serial.println("Print Loop Start");

        delay(5000);

        Serial.println("Print Loop End");

        delay(5000);
    }
} print_task;

class BlinkLeanTask : public LeanTask {
protected:
    void setup() {
        state = HIGH;

        pinMode(2, OUTPUT);
        pinMode(2, state);
    }

    void loop() {
        state = state == HIGH ? LOW : HIGH;
        pinMode(2, state);
        // lean tasks don't have a delay, but they consume no extra ram
        // use schedule(ms) anywhere in your loop to indicate that the next cycle of the loop
        // should happen x milliseconds in the future 
        schedule(1000);
    }

private:
    uint8_t state;
} blink_leanTask;

class MemTask : public Task {
public:
    void loop() {
        Serial.print("Free Heap: ");
        Serial.print(ESP.getFreeHeap());
        Serial.println(" bytes");

        delay(10000);
    }
} mem_task;

void setup() {
    Serial.begin(115200);

    Serial.println("");

    delay(1000);

    Scheduler.start(&print_task);
    Scheduler.start(&blink_leanTask);
    Scheduler.start(&mem_task);

    Scheduler.begin();
}

void loop() {}
