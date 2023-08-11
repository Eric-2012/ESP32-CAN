/******************************************************************************
 * class_TaskManager.h
 * Header for TaskManager.cpp
 *****************************************************************************/
#ifndef __CLASS_TASKMANAGER_H__
#define __CLASS_TASKMANAGER_H__

#include "Arduino.h"

class TaskManager
{
private:
	// Task parameters
	// Used in the function xTaskCreatePinnedToCore
	TaskFunction_t taskCode;
	String name;
	uint32_t stackDepth;
	uint32_t interval;
	uint8_t priority;
	uint8_t core;

	// Debug variables
	uint32_t initTime = 0;		 // Variable used to save the time when the task was started
	uint32_t numExec = 0;		 // Number of times that the task was executed
	uint32_t lastNumExec = 0;	 // Last number of times that the task was executed
	uint32_t lastExecTime = 0;	 // Time it took the last time the task was executed
	uint32_t maxExecTime = 0;	 // Max time that the task took to be executed since the board was powered on
	uint32_t lastStackDepth = 0; // Stack size of the last time the task was executed
	uint32_t maxStackDepth = 0;	 // Max stack size that the task used to be executed since the board was powered on

	// Some commands are the same for core 0 and 1
	void run();

public:
	// Task handler
	TaskHandle_t handler;

	// Class constructors
	TaskManager();
	~TaskManager();

	// Configure the task to run in Core0
	void runInCore0(TaskFunction_t _taskCode,
					String _name,
					uint32_t _stackDepth,
					uint32_t _interval,
					uint8_t _priority);

	// Configure the task to run in Core1
	void runInCore1(TaskFunction_t _taskCode,
					String _name,
					uint32_t _stackDepth,
					uint32_t _interval,
					uint8_t _priority);

	// Pause the task
	void pause();

	// Resume the task
	void resume();

	// Get the interval that the task needs to sleep, based on the execution time
	uint32_t getInterval();

	// Restart the timer in the beggining of the task
	void initDebugExec();

	// Increment execution counter, calc execution time, calc stack depth
	void endDebugExec(uint32_t freeStackDepth);

	// Get number of times that the task was executed since the board was powered on
	uint32_t getNumExec();

	// Get number of times that the task was executed since time that this function was called
	uint8_t getNumExecPart();

	// Get the time that the last time took to run
	uint32_t getExecLastTime();

	// Get the maximum time that the task took to run since the board was powered on
	uint32_t getExecMaxTime();

	// Get the stack depth that the last task used
	uint32_t getCurrentStackDepth();

	// Get the maximum stack size that the tasks used since the board was powered on
	uint32_t getMaxStackDepth();

	// Get the string ready to be used as datalog
	String getDatalogString();
};
#endif