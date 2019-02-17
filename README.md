# DOCUMENTATION
## Tasks
This module contains methods for **Tasks**, an attempt at concurrency:


* A **Task** is a combination of **an action function and a closure function** (both parameterless), **and a Timer**.
  * A Task triggers its action function **after n amount of time**, then triggers its closure function **after m amount of time** **(from the time it was called)**: once, forever, or n times (excluding 0ms).

## Using the module

To create a Task, you must first create an **action function and its corresponding closure function** (both **parameterless**) in the **global scope**, ex:
```c
void turnOnRelay(void){
    ...
}
void turnOffRelay(void){
    ...
}
```

Then, you must create the corresponding Timer structure **globally**, ex:
```c
...
TaskTimer relayTimer = {.f_ms=2000, .cf_ms=5000, .callTime=0, .ACTION=1, .REPEAT=-1};
...
```
or do it in setup, ex:
```c
setup(){
  setTaskTimer(&relayTimer, 2000, 5000, -1);
  ...
}
```

where:
  ```c
  void setTaskTimer(TaskTimer *taskTimer, f_ms, cf_ms, REPEAT);
  ```

* **f_ms** refers to the interval of time before the action function is activated.
  * i.e **how long does the Task stay inactive?**

* **cf_ms** refers to the interval of time before the closure function is activated.
  * i.e **how long does the Task stay active?**

* **REPEAT** has the following options:
  * **=n** : Do this Task **n** times
  * **=-1** : Repeat this Task forever.

Finally, you can enqueue the Task inside the Arduino loop():
```c
void loop() {
  enqueueTask(turnOnRelay, turnOffRelay, &relayTimer);
  ...
}
```
**NOTE**: If you would like to use functions with parameters, then you need to
apply the logic found in the enqueue functions manually.

**WARNING**: These methods are bound by the loop() iteration duration:
If your Timer is set to 40s, but the iteration has a 50s delay after the method is
called, then your Event/Task will be triggered after 50s, not 40s.

**WARNING**: The internal clock (milis) is an unsigned long int, thus it overflows
at 4,294,967,295 (2^32 - 1, unsigned long max) (roughly 50 days) before 
rolling over to 0. This means that you should somewhat expect unexpected behaviour
if you leave the Arduino on for that long.

# Creating libraries for the Arduino
To create libraries, you must create a folder **\<Library\>** containing:

    1. <Library>.h -> Your header file
    2. <Library>.cpp -> Your code

Then you must place **\<Library\>** inside **Documents/Arduino/libraries**.
You should then be able to load them inside the IDE with **Sketch->Include Library**

**NOTE**: The module must be in **.cpp** or else you must put the header file inside an ugly wrapper, check out https://stackoverflow.com/questions/19635007/arduino-c-undefined-reference-to-readarms for more info.