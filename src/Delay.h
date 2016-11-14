#ifndef DELAY_H
#define DELAY_H

#include <Arduino.h>

class Delay {
public:
    inline void reset() {
        delay_ms = 0;
    }

    void set(uint32_t ms) {
        delay_start = millis();
        delay_ms = ms;
    }

    bool update(uint32_t time) {
        if (delay_ms == 0) return false;

        if ((time - delay_start) >= delay_ms) {
            reset();
        }

        return delay_ms != 0;
    }

protected:
    uint32_t delay_start = 0;
    uint32_t delay_ms = 0;
};

#endif
