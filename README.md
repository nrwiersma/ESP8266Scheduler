![Logo](http://svg.wiersma.co.za/github/project?lang=cpp&title=ESP8266Scheduler&tag=co-operative%20multitasking)

# Quick Start

## Installing

You can install through the Arduino Library Manager. The package name is
**ESP8266Scheduler**.

## Usage

Include the library in your sketch

```cpp
#include <Scheduler.h>
```

In your setup function start the scheduler

```cpp
Scheduler.start(&task);

Scheduler.begin();
```

The scheduler blocks once begun, so the loop function is never called. You should instead create tasks.

# Creating a Task

Tasks are classes that should inherit the ```Task``` class. A task can define a ```loop()``` and ```setup()``` function
much as the normal Arduino standard.

```cpp
class BlinkTask : public Task {
protected:
    void setup() {
        state = HIGH;

        pinMode(2, OUTPUT);
        pinMode(2, state);
    }

    void loop() {
        state = state == HIGH ? LOW : HIGH;
        pinMode(2, state);

        delay(1000);
    }

private:
    uint8_t state;
} blink_task;
```

**IMPORTANT: Tasks must be declared globally on the stack (not a pointer). Failure to do so will crash your device.**

Tasks can run ```yield``` and ```delay``` like they normally would. These functions yield control to the scheduler
rather than the ESP8266.

**IMPORTANT: Each task consumes 4kb of ram.**

### Advanced Task Functions

The ```Task``` also exposes a ```bool shouldRun()``` method that is used determine if the task loop
should be be resumed. This can be inherited to add your own logic to determine if your code should be resumed.

```cpp
bool shouldRun() {
    bool run = Task::shouldRun();

    // Your code here

    return run;
}
```

**This function handles the ```delay()``` logic. The parent method should be called.**

# Creating a LeanTask

A ```LeanTask``` runs in the global context, so calling ```delay()```  inside one blocks execution for all tasks. The advantage of lean tasks is that they consume no extra RAM, so you can add as many as you wish.
LeanTasks can run ```schedule(ms);``` to indicate to the scheduler, that the next iteration of their loop function should not run for the next `ms` milliseconds.

If you have a ```Task``` that uses one single call to `delay(ms)` at the end of the loop, you can save 4k of ram by declaring it as a ```LeanTask``` instead and replacing `delay(ms)` with `schedule(ms)` call.

```cpp
class BlinkTask : public LeanTask {
protected:
    void setup() {
        state = HIGH;

        pinMode(2, OUTPUT);
        pinMode(2, state);
    }

    void loop() {
        state = state == HIGH ? LOW : HIGH;
        pinMode(2, state);

        schedule(1000);
    }

private:
    uint8_t state;
} blink_task;
```


# Documentation

## Methods

### start
```
static void start(Task *task)
static void start(LeanTask *leanTask)
```
> Adds a task to the multitasking queue.

### begin
```
static void begin()
```
> Starts the scheduler. This function is "blocking". It should be the last call the ```setup``` function.
