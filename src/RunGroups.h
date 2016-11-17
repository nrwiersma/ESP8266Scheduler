#ifndef RUN_GROUPS_H
#define RUN_GROUPS_H

#include <Arduino.h>

struct RunGroupInfo {
    uint8_t id;
    bool complete;

    RunGroupInfo() {
        id = 0xFF;
        complete = false;
    }

    void reset() {

    }
 };

 class RunGroups {
 public:
    static bool current(RunGroupInfo info);

     static bool defaultGroup(RunGroupInfo info);

     static void next();

 private:
     static uint8_t current_id;
     static uint8_t next_id;
     static uint8_t lowest_id;
 };

 #endif
