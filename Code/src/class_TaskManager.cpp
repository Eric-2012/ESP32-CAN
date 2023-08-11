/******************************************************************************
 * TaskManager.cpp
 * This file has functions to be used in the tasks management
 *****************************************************************************/

#include "class_TaskManager.h"

// Class constructors
TaskManager::TaskManager(){};
TaskManager::~TaskManager(){};

// Configure the task to run in Core0
void TaskManager::runInCore0(TaskFunction_t _taskCode,
                             String _name,
                             uint32_t _stackDepth,
                             uint32_t _interval,
                             uint8_t _priority)
{
    taskCode = _taskCode;
    name = _name;
    stackDepth = _stackDepth;
    interval = _interval;
    priority = _priority;
    core = 0;

    // Run the task
    run();
}

// Configure the task to run in Core1
void TaskManager::runInCore1(TaskFunction_t _taskCode,
                             String _name,
                             uint32_t _stackDepth,
                             uint32_t _interval,
                             uint8_t _priority)
{
    taskCode = _taskCode;
    name = _name;
    stackDepth = _stackDepth;
    interval = _interval;
    priority = _priority;
    core = 1;

    // Run the task
    run();
}

// Start the task
void TaskManager::run()
{
    // Convert the task name to char array
    char buffer[30];
    name.toCharArray(buffer, 30);
    const char *const auxName = buffer;

    // It was decided to work as priority 0 being the most important, so it is necessary to convert,
    // because for FreeRTOS (configMAX_PRIORITIES - 1) means high priority and 0 means low priority
    uint8_t auxPriority;
    if (priority >= 24)
        auxPriority = 0;
    else
        auxPriority = (configMAX_PRIORITIES - 1) - priority;

    // Start the task
    xTaskCreatePinnedToCore(taskCode,
                            auxName,
                            stackDepth,
                            0,
                            auxPriority,
                            &handler,
                            core);
}

// Pause the task
void TaskManager::pause()
{
    vTaskSuspend(handler);
}

// Resume the task
void TaskManager::resume()
{
    vTaskResume(handler);
}

// Get the interval that the task needs to sleep, based on the execution time
uint32_t TaskManager::getInterval()
{
    // The execTime is micros, but the interval is millis, so it is necessary to convert first
    uint32_t auxExecTime = lastExecTime / 1000;

    // If the task took less time than the interval, it sleeps just the difference, to keep a fixed execution time
    // If the task took more time than the interval, it sleeps just for 1ms
    if (interval > auxExecTime)
        return interval - auxExecTime;
    else
        return 1;
}

// Restart the timer in the beggining of the task
void TaskManager::initDebugExec()
{
    // Get the time that the task started
    initTime = micros();
}

// Increment execution counter, calc execution time, calc stack depth
void TaskManager::endDebugExec(uint32_t freeStackDepth)
{
    // Increment execution counter
    numExec++;

    // Calc execution time
    lastExecTime = micros() - initTime;

    // Check the maximum execution time
    if (lastExecTime > maxExecTime)
        maxExecTime = lastExecTime;

    // Calc stack size
    lastStackDepth = stackDepth - freeStackDepth;

    // Check the maximum stack depth
    if (lastStackDepth > maxStackDepth)
        maxStackDepth = lastStackDepth;
}

// Get number of times that the task was executed since the board was powered on
uint32_t TaskManager::getNumExec()
{
    return numExec;
}

// Get number of times that the task was executed since time that this function was called
uint8_t TaskManager::getNumExecPart()
{
    // Calculates the number on executions since last time that this funcion was called
    uint32_t aux = numExec - lastNumExec;

    // Limits in 255
    if (aux > 255)
        aux = 255;

    // Updates the lastNumExec
    lastNumExec = numExec;

    // Return the number
    return (uint8_t)aux;
}

// Get the time that the last time took to run
uint32_t TaskManager::getExecLastTime()
{
    return lastExecTime;
}

// Get the maximum time that the task took to run since the board was powered on
uint32_t TaskManager::getExecMaxTime()
{
    return maxExecTime;
}

// Get the stack depth that the last task used
uint32_t TaskManager::getCurrentStackDepth()
{
    return lastStackDepth;
}

// Get the maximum stack size that the tasks used since the board was powered on
uint32_t TaskManager::getMaxStackDepth()
{
    return maxStackDepth;
}

// Get the string ready to be used as datalog
String TaskManager::getDatalogString()
{
    String aux = "";
    aux.concat(numExec);
    aux.concat(";");
    aux.concat(maxStackDepth);
    aux.concat(";");

    return aux;
}