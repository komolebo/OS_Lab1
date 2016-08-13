#include "Task.h"

unsigned int Task::idCommon = 0;

Task::Task(int _time1, int _time2) : time1(_time1), time2(_time2), id(idCommon++), timeToDo(_time2) {}

Task::~Task() {}

unsigned int Task::getID() 
{
	return id;
}

void Task::execute(int t, int tact)
{
	timeToDo -= t;

	// if task is Done
	if (timeToDo <= 0) time3 = t + tact;
}

bool Task::isPerformed()
{
	return timeToDo <= 0;
}

unsigned int Task::getTime(int i)	// Simple getter
{ 
	switch (i)
	{
	case 1:
		return time1;		// Joined
	case 2:
		return time2;		// Task time
	case 3:
		return time3;		// Done
	case 4:
		return timeToDo;	// Left
	case 5:
		return time4;		// Delay
	default:
		break;
	}
}

bool operator== (const Task & task1, const Task & task2)
{
	return task1.id == task2.id;
}

void Task::delay()
{
	time4++;
}