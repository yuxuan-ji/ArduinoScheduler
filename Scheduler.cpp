#include "Scheduler.h"
#include "Arduino.h"

void enqueueEvent(void (*f)(void), EventTimer *eventTimer)
{   
    int REPEAT = (eventTimer -> REPEAT);
    if (REPEAT == 0) {return;}

    //Update callTime to be the time the method was called.
    if ((eventTimer -> callTime) == 0) {(eventTimer -> callTime) = millis();}

    unsigned long ms = (eventTimer -> ms);
    
    //An event triggers each time at least ms has passed since the method was called:
    //NOTE: millis() should always be bigger than callTime
    if (((millis() - (eventTimer -> callTime)) >= ms))
    {
        //REPEAT_N_TIMES event:
        if (REPEAT > 0)
        {
            (eventTimer -> callTime) = millis();
            (eventTimer -> REPEAT) -= 1;  //Decrement eventTimer.REPEAT until 0:DONE
            (*f)();
        }
        //REPEAT_FOREVER event:
        else if (REPEAT == -1)
        {
            (eventTimer -> callTime) = millis();
            (*f)();
        }   
    }
}

void setTaskTimer(TaskTimer *taskTimer, unsigned long f_ms, unsigned long cf_ms, int REPEAT)
{
    (taskTimer -> f_ms) = f_ms;
    (taskTimer -> cf_ms) = cf_ms;
    (taskTimer -> REPEAT) = REPEAT;
    (taskTimer -> callTime) = 0;
    (taskTimer -> ACTION) = 1;
}

void enqueueTask(void (*f)(void), void (*cf)(void), TaskTimer *taskTimer)
{   
    int REPEAT = (taskTimer -> REPEAT);
    if (REPEAT<=0) {return;}
    
    //Update callTime to be the time the method was called.
    if ((taskTimer -> callTime) == 0) {(taskTimer -> callTime) = millis();}

    //Associate the correct interval ms depending on the action:
    //Recall that {0:CLOSURE_FUNCTION, 1:FUNCTION}
    int ACTION = (taskTimer -> ACTION);
    unsigned long ms = ACTION ? (taskTimer -> f_ms) : (taskTimer -> cf_ms);
    
    //A task triggers each time at least ms has passed since the method was called:
    //NOTE: millis() should always be bigger than callTime
    if ((millis() - (taskTimer -> callTime)) >= ms)
    {
        //REPEAT_N_TIMES task:
        if (REPEAT > 0)
        {   
            //Decrement taskTimer.REPEAT each time the closure function is activated until 0:DONE
            if (!(taskTimer -> ACTION)) {(taskTimer -> REPEAT) -= 1;}

            //Call the appropriate function, then update the next function to call:
            ACTION ? (*f)() : (*cf)();
            (taskTimer -> ACTION) ^=1;  //flip 0 to 1 and viceversa
            (taskTimer -> callTime) = millis(); 
        }
        //REPEAT_FOREVER task:
        else if (REPEAT == -1)
        {
            //Call the appropriate function, then update the next function to call:
            ACTION ? (*f)() : (*cf)();
            (taskTimer -> ACTION) ^=1;  //flip 0 to 1 and viceversa
            //Update method callTime for the next function:
            (taskTimer -> callTime) = millis();
        }
    }
}