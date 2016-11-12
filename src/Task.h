#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
#include "Scheduler.h"

extern "C" {
    #include "cont.h"
}

class Task {
public:
    Task() {
        cont_init(&context);
    }

protected:
    virtual void setup() {}

    virtual void loop() {}

    void delay(unsigned long ms) {
        if (ms)
            delay_done_ms = millis() + ms;

        yield();
    }

    void yield() {
        cont_yield(&context);
    }

    virtual bool shouldRun() {
        return (millis() >= delay_done_ms);
    }

private:
    friend class SchedulerClass;
    friend void task_tramponline();

    Task *next;
    Task *prev;
    cont_t context;

    bool setup_done = false;
    unsigned long delay_done_ms = 1;

    void loopWrapper() {
        if (!setup_done) {
            setup();
            setup_done = true;
        }

        while(1) {
            loop();
            yield();
        }
    }
};

#endif
