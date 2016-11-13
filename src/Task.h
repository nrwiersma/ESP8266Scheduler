#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
//#include "Scheduler.h"

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
        if (ms) {
            delay_start = millis();
            delay_ms = ms;
        }

        yield();
    }

    void yield() {
        cont_yield(&context);
    }

	inline bool isDelayed() {
		return (delay_ms != 0);
	}

	void updateDelayTimer() {
		if (delay_ms == 0) return;   // Optimize for the non-delayed case

		// This comparison is "rollover safe"
        unsigned long now = millis();
        if ((now - delay_start) >= delay_ms)
			delay_ms = 0;
	}

    virtual bool shouldRun() {
		// Tasks update their own delay timer
		updateDelayTimer();
		if (isDelayed()) return false;
        if (!run_group_active) return false;
		return !loop_complete;
	}

    uint8_t current_cycle_id = 0;
    uint8_t run_group_id = 0xFF;
    bool run_group_active = false;
	
    bool loop_complete = false;
private:
    friend class SchedulerClass;
    friend void task_tramponline();

    Task *next;
    Task *prev;
    cont_t context;


    bool setup_done = false;
    unsigned long delay_start = 0;
    unsigned long delay_ms = 0;

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
