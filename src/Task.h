#ifndef TASK_H
#define TASK_H

#include "Delay.h"
#include "RunGroups.h"

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
		return true;
	}
private:
    friend class SchedulerClass;
    friend void task_tramponline();

    Task *next;
    Task *prev;
    cont_t context;

    bool setup_done = false;

    Delay delay_info;
    RunGroupInfo run_group_info;

    void loopWrapper() {
        if (!setup_done) {
            setup();
            setup_done = true;
        }

        while(1) {
            if (shouldRun()) {
                run_group_info.complete = loop();
            } else {
                // We are not ready, give the other run groups a turn.
                run_group_info.complete = true;
            }

            yield();
        }
    }
};

#endif
