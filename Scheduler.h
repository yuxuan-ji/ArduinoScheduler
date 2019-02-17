#ifndef SCHEDULER_H
#define SCHEDULER_H

/*
This module contains methods for Events and Tasks, an attempt at concurrency:
    -An Event is a combination of a parameterless function and a Timer.
    -An Event is triggered once every x ms (from the time it was called): forever, or n times (excluding 0ms).
    -A Task is a combination of an action function and a closure function (both parameterless), and a Timer.
    -A Task triggers its action function after x f_ms, then triggers its closure function after y cf_ms (from the time it was called): forever, or n times (excluding 0ms).
*/

typedef struct EventTimer {
    unsigned long ms; //Interval ms
    unsigned long callTime; //Should always be set to 0
    int REPEAT;  //{N:REPEAT_N_TIMES, -1:REPEAT_FOREVER}
} EventTimer;

typedef struct TaskTimer {
    unsigned long f_ms;
    unsigned long cf_ms;
    unsigned long callTime; // -> Should always be set to 0.
    int ACTION; //{0:CLOSURE_FUNCTION, 1:FUNCTION} -> Should always be set to 1.
    int REPEAT;
} TaskTimer;

void setTaskTimer(TaskTimer *taskTimer, unsigned long f_ms, unsigned long cf_ms, int REPEAT);


void enqueueEvent(void (*f)(void), EventTimer *eventTimer);

void enqueueTask(void (*f)(void), void (*cf)(void), TaskTimer *taskTimer);

/*
NOTE: If you would like to use functions with parameters, then you need to
apply the logic found in the enqueueEvent function manually.

WARNING: These methods are bound by the loop() iteration duration:
If your Timer is set to 40s, but the iter has a 50s delay after the method is
called, then your Event/Task will be triggered after 50s, not 40s.

WARNING: The internal clock (milis) is an unsigned long int, thus it overflows
at 4,294,967,295 (2^32 - 1, unsigned long max) (roughly 50 days) before 
rolling over to 0. This means that you should somewhat expect unexpected behaviour
if you leave the Arduino on for that long.
*/

#endif