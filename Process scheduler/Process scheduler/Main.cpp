#include "Task.h"
#include <list>
#include <stdlib.h>		// rand, system("cls")
#include <Windows.h>	// sleep
#include <fstream>

using namespace std;

void addTasks(list<Task> & queue, unsigned int tact);
void output(list<Task> queue, list<Task> logs, char * tableName);
void outfile(list<Task> logs, char * tableName);

Settings settings;

int main()
{
	list<Task> queueBackground, queueInteractive;
	list<Task> logsBackround, logsInteractive;
	unsigned int tact = 0;
	unsigned int bufI = 0; 
	int bufB = -1;
	
	while (settings.tasksNumber || !queueBackground.empty() || !queueInteractive.empty())
	{
		// Adding tasks to queue
		addTasks(queueBackground, tact);
		addTasks(queueInteractive, tact);

		// Processor works differently depends on tact
		// 80% - interactive, 20% - background ->		only 4th, 9th, 14th, 19th, ... are background
		if (  ((tact + 1) % 5 == 0 || queueInteractive.empty()) && !queueBackground.empty() )	// tact of background process
		{
			list<Task>::iterator it = queueBackground.begin();

			if (bufB < 0)
				bufB = queueBackground.size() - 1;

			advance(it, bufB);
			(*it).execute(1, tact);

			///queueBackground.front().execute(1, tact);

			// Delay all other processes
			for (auto & t : queueBackground)
				if ( !(t == queueBackground.front()))
					t.delay();
			for (auto & t: queueInteractive)
				t.delay();

			if ((*it).isPerformed())		
			{
				logsBackround.push_back(*it);
				queueBackground.remove(*it);
				bufB = -1;
			}
		} 
		else if (!queueInteractive.empty())
		{
			if (bufI >= queueInteractive.size()) bufI = 0;

			// get task according to RR algourithm
			list<Task>::iterator it = queueInteractive.begin();
			advance(it, bufI++);

			(*it).execute(1, tact);

			// Delay all other processes
			for (auto & t : queueInteractive)
				if (!(t == *it))
					t.delay();
			for (auto & t : queueBackground)
				t.delay();

			if ((*it).isPerformed())
			{
				logsInteractive.push_back(*it);
				queueInteractive.remove(*it);
			}
		}

		// Clear screen
		system("cls");
		// Output background tasks info
		output(queueBackground, logsBackround, "Background tasks manager");
		// Output interactive tasks info
		output(queueInteractive, logsInteractive, "Interactive tasks manager");
		printf("\nCurrent tact:  %4d\nTasks will come: %2d", tact, settings.tasksNumber);

		Sleep(settings.tactTimeInMS);
		tact++;
	}

	outfile(logsBackround, "Background tasks");
	outfile(logsInteractive, "Interactive tasks");

	return 0;
}

void addTasks(list<Task> & queue, unsigned int tact)
{
	if (!settings.tasksNumber) return;

	// Decide how many tasks should we add per tact
	unsigned int N = rand() % (settings.tasksMaxPerTact + 1);

	// If not many task are we allowed to add
	N = settings.tasksNumber < N ? settings.tasksNumber : N;

	while (N)
	{
		queue.push_back(Task(tact, rand() % settings.taskMaxTime + 1));
		N--;
		settings.tasksNumber--;
	}
}

void output(list<Task> queue, list<Task> logs, char * tableName)
{
	printf("%s\n\n", tableName);
	printf("%8s %12s %12s %12s %12s %12s\n", "ID", "Join", "Task time", "Done", "Left", "Delay");
	// Firstly output done tasks info
	for (auto t : logs)
		printf("%8d %12d %12d %12d %12d %12d\n", t.getID(), t.getTime(1), t.getTime(2), t.getTime(3), t.getTime(4), t.getTime(5));

	// Then output processing tasks info
	for (auto t : queue)
		printf("%8d %12d %12d %12d %12d %12d\n", t.getID(), t.getTime(1), t.getTime(2), t.getTime(3), t.getTime(4), t.getTime(5));
	printf("\n");
}

void outfile(list<Task> logs, char * tableName)
{
	ofstream f;
	f.open("Statistics.txt", std::ios_base::app);

	if (f.is_open())
	{
		f << tableName << endl;
		f << "ID	Join	Task time	Done	Delay" << endl;
		// Output processes info
		for (auto t : logs)
			f << t.getID() << '\t' << t.getTime(1) << '\t' << t.getTime(2) << "\t\t" << t.getTime(3) << '\t' << t.getTime(5) << endl;

		// Output averages
		unsigned int averageTaskTime = 0, averageDone = 0, averageDelay = 0, 
			maxJoin = logs.front().getTime(1), minJoin = logs.front().getTime(1);
		for (auto t : logs)
		{
			averageTaskTime += t.getTime(2);
			averageDone += t.getTime(3);
			averageDelay += t.getTime(5);
			minJoin = t.getTime(1) < minJoin ? t.getTime(1) : minJoin;
			maxJoin = t.getTime(1) > maxJoin ? t.getTime(1) : maxJoin;
		}
		f << "Task time average:   " << (double)averageTaskTime / logs.size() << endl;
		f << "Done average:        " << (double)averageDone / logs.size() << endl;
		f << "Delay     average:   " << (double)averageDelay / logs.size() << endl;
		f << "All tasks:           " << logs.size() << endl;
		f << "Estimated diffulty:  " << (double)averageTaskTime << endl << endl;

		f.close();
	}
}