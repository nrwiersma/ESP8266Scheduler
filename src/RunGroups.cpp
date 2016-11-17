#include "RunGroups.h"

uint8_t RunGroups::current_id;
uint8_t RunGroups::next_id = 0xFF;
uint8_t RunGroups::lowest_id = 0xFF;

bool RunGroups::current(RunGroupInfo info) {
    // We need to keep track of the lowest id
    if (info.id < lowest_id) {
        lowest_id = info.id;
    }

    // We should work out what our next run group id is
    if (info.id > current_id && info.id < next_id) {
        next_id = info.id;
    }

    // If it is in the default run group, you can always run
    if (info.id == 0xFF) return true;

    // Decide if we can run in this round
    return !info.complete && info.id == current_id;
 }

 bool RunGroups::defaultGroup(RunGroupInfo info) {
     return info.id == 0xFF;
 }

 void RunGroups::next() {
    if (next_id == 0xFF) {
        current_id = lowest_id;
    } else {
        current_id = next_id;
        next_id = 0xFF;
    }
 }
