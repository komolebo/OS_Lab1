class Task
{
public:
	Task();
	Task(int, int);
	~Task();

	bool isPerformed();
	unsigned int getID();
	unsigned int getTime(int );	// simple getter
	void execute(int, int);		// execute task for exact time
	void delay();				// delay, while task is not working

	friend bool operator==(const Task & left, const Task & right);
private:
	unsigned int id;
	static unsigned int idCommon;

	/* time1 - joined,		time2 - time to work on,		time3 - done,		time4 - delay */
	unsigned int time1, time2, time3 = 0, time4 = 0, timeToDo = 0;
};

struct Settings
{
	unsigned int tasksNumber = 10;
	unsigned int tasksMaxPerTact = 1;
	unsigned int taskMaxTime = 7;
	unsigned int tactTimeInMS = 300;
};