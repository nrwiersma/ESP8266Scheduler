![Logo](http://svg.wiersma.co.za/github/project?lang=cpp&title=ESP8266Scheduler&tag=co-operative%20multitasking)

[![Build Status](https://github.com/nrwiersma/ESP8266Scheduler/actions/workflows/test.yml/badge.svg)](https://github.com/nrwiersma/ESP8266Scheduler/actions)
[![arduino-library-badge](http://www.ardu-badge.com/badge/ESP8266Scheduler.svg)](http://www.ardu-badge.com/ESP8266Scheduler)

# Quick Start

## Installing

You can install through the Arduino Library Manager. The package name is
**ESP8266Scheduler**.

## Usage

Include the library in your sketch

```cpp
#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>
```

In your setup function start the scheduler

```cpp
Scheduler.start(&task);

Scheduler.begin();
```

The scheduler blocks once begun, so the loop function is called by the scheduler BEFORE the first task is runned each time. But it is recommended to avoid using a loop.

## Creating a Task

Tasks are classes that should inherit the ```Task``` class. A task can define a ```loop()``` and ```setup()``` function
much as the normal Arduino standard.

```cpp
class BlinkTask : public Task {
 protected:
  void setup() {
    state = HIGH;

    pinMode(2, OUTPUT);
    digitalWrite(2, state);
  }

  void loop() {
    state = state == HIGH ? LOW : HIGH;
    digitalWrite(2, state);

    delay(1000);
  }

private:
  uint8_t state;
} blink_task;
```

**IMPORTANT:** Tasks must be declared globally on the stack (not a pointer). Failure to do so will crash your device.

Tasks can run ```yield()``` and ```delay()``` like they normally would. These functions yield control to the scheduler
rather than the ESP8266.

## Creating a LeanTask

Lean tasks are classes that should inherit the ```LeanTask``` class. A lean task can define a ```loop()``` and ```setup()``` function
much as the normal Arduino standard.

LeanTask doesn't use ```cont.h``` (runs in the global context), so ```yield()``` works exactly as it would without using the library. This can be useful for saving ram (4k) if the task does not use ```yield()``` to interrupt the task. You can use ```delay()``` at the end of a task to set the interval.

**Good example:**
```cpp
class MemTask : public LeanTask {
 public:
  void loop() {
    Serial.print("Free Heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");

    delay(10000);
  }
} mem_task;
```

**Bad example. The first ```delay()``` does nothing:**
<details><summary>Source. DO NOT DO THIS!</summary>
<p>

```cpp
class PrintTask : public LeanTask {
 protected:
  void loop() {
    Serial.println("Print Loop Start");

    delay(5000);

    Serial.println("Print Loop End");

    delay(5000);
  }
} print_task;
```

</p>
</details>

Tasks can run ```yield()``` and ```delay()``` like they normally would. The ```yield()``` function transfers control to the ESP8266, NOT the scheduler. The ```delay()``` function will tell the scheduler that a delay is needed before the next run. If you REALLY need a delay, use ```::delay()```, but this will block the task and the scheduler.

## Tests using Task vs LeanTask
All examples have the same logic. To optimize RAM, use ```LeanTask``` (if possible), because each instance of ```Task``` requires 4 kb of RAM.
| File | Description | Free heap (more is better) |
| --- | --- | --- |
| [simple.ino](examples/simple/simple.ino) | 3 Task | 39896 bytes |
| [lean_simple.ino](examples/lean_simple/lean_simple.ino) | 1 Task, 2 LeanTask | 48168 bytes |
| [subfile_simple.ino](examples/subfile_simple/subfile_simple.ino) | 1 Task, 2 LeanTask + main loop | 48136 bytes |

[heap_test.ino](examples/heap_test/heap_test.ino):
| TASK_TYPE | Description | Free heap (more is better) |
| --- | --- | --- |
| Task | 12 Task + main loop | 2280 bytes |
| LeanTask | 12 LeanTask + main loop | 51912 bytes |

## Task methods
#### bool AbstractTask::isEnabled();
Method return the status of the task: enabled (true) or disabled (false).

---

#### AbstractTask(bool _enabled = true, unsigned long _interval = 0);
Constructor.

---

#### bool AbstractTask::isEnabled();
Method return the status of the task: enabled (true) or disabled (false).

---

#### void AbstractTask::enable();
The method enable the task.

---

#### void AbstractTask::disable();
The method disable the task.

---

#### void AbstractTask::setInterval(unsigned long val);
The method sets the task run interval.

---

#### unsigned long AbstractTask::getInterval();
The method returns the task run interval.


## Scheduler methods
#### static void SchedulerClass::start(AbstractTask *task);
Adds a task to the multitasking queue.

---

#### static void SchedulerClass::begin();
Starts the scheduler. This function is "blocking". It should be the last call the ```setup``` function.

---

#### static void SchedulerClass::delay(unsigned long ms);
Calls ```AbstractTask::delay()``` on the runned task from outside

**IMPORTANT:** You should not call this method outside for LeanTask tasks. It does not make sense.

---

#### static void SchedulerClass::yield();
Calls ```AbstractTask::yield()``` on the runned task from outside

**IMPORTANT:** You should not call this method outside for LeanTask tasks. It does not make sense.
