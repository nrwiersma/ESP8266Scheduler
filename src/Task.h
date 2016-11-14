#ifndef TASK_H
#define TASK_H

#include "Delay.h"

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

    virtual bool loop() { return true; }

    void delay(uint32_t ms) {
        delay_info.set(ms);

        yield();
    }

    void yield() {
        cont_yield(&context);
    }

    virtual bool shouldRun() {
		if (delay_info.update(millis())) {
            return false;
        }

        if (!run_group_active) return false;

		return !loop_complete;
	}

    uint8_t current_cycle_id = 0;
    uint8_t run_group_id = 0xFF;
    bool run_group_active = false;
private:
    friend class SchedulerClass;
    friend void task_tramponline();

    Task *next;
    Task *prev;
    cont_t context;

    Delay delay_info;

    bool setup_done = false;
    bool loop_complete = false;

    void loopWrapper() {
        if (!setup_done) {
            setup();
            setup_done = true;
        }

        while(1) {
            loop_complete = loop();
            yield();
        }
    }
};

#endif
