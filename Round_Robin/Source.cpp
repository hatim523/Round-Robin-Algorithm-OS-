#include<iostream>
#include<queue>
#include<string.h>

using namespace std;

void ArrangeProcesses(double **arrival_time, int **burst_time, int total_process);

static int process_id = 5200;

int main()
{
	int total_process;
	cout << "Enter number of Processes: ";
	cin >> total_process;

	double qt;
	cout << "Enter Quantum Time: ";
	cin >> qt;

	double **arrival_time = new double*[total_process];
	int **burst_time = new int*[total_process];

	for (int i = 0; i < total_process; i++)		//first column for process ID (generated randomly) and second column for relevant data
	{
		arrival_time[i] = new double[2];
		burst_time[i] = new int[3];		//3rd col is replica of 2nd col	|| 3rd col is constant for reference purposes.
	}

	int total_burst_time = 0;

	for (int i = 0; i < total_process; i++)
	{
		cout << "Process P" << i << " | Arrival Time: ";
		cin >> arrival_time[i][1];
		cout << "Process P" << i << " | Burst Time: ";
		cin >> burst_time[i][1];
		total_burst_time += burst_time[i][1];
		burst_time[i][2] = burst_time[i][1];	//not tested
		arrival_time[i][0] = process_id;
		burst_time[i][0] = process_id;
		process_id++;
	}

	//sorting arrival_times along with their burst times
	ArrangeProcesses(arrival_time, burst_time, total_process);

	cout << endl << "Process\t\tArrival Time\t\tBurst Time" << endl;
	for (int i = 0; i < total_process; i++)
		cout << "P" << burst_time[i][0] << "\t\t" << arrival_time[i][1] << "\t\t\t" << burst_time[i][1] << endl;

	queue<double> q_arrival;
	queue<int> q_burst;
	queue<int> q_id;
	queue<int> q_burst_const;

	//initializing array for completion time, Turn Around Time, and Waiting Time
	double **process_time = new double*[total_process];
	for (int i = 0; i < total_process; i++)
	{
		process_time[i] = new double[4];	//1st Col = Completion Time || 2nd Col = TAT || 3rd Col = WT  || 4th col = Process ID
		memset(process_time[i], 0, sizeof(double) * 4);
	}

	int time = 0;
	int j = 0;
	time += arrival_time[0][1];
	while (j < total_process && arrival_time[j][1] <= time)	//populating queue with the arrived processes
	{
		q_arrival.push(arrival_time[j][1]);
		q_burst.push(burst_time[j][1]);
		q_id.push(arrival_time[j][0]);
		q_burst_const.push(burst_time[j][2]);
		j++;
	}

	while (!q_id.empty())
	{
		while (j < total_process && arrival_time[j][1] <= time)	//populating queue with the arrived processes
		{
			q_arrival.push(arrival_time[j][1]);
			q_burst.push(burst_time[j][1]);
			q_id.push(arrival_time[j][0]);
			q_burst_const.push(burst_time[j][2]);
			j++;
		}
		//executing the process in queue
		if (!q_id.empty())
		{
			int current_process_arrival_time = q_arrival.front();
			q_arrival.pop();
			int current_process_burst_time = q_burst.front();
			q_burst.pop();
			int current_process_id = q_id.front();
			q_id.pop();
			int const_burst_time = q_burst_const.front();
			q_burst_const.pop();

			if (current_process_burst_time < qt)
			{
				time += current_process_burst_time;
				//finding empty space
				int loc = 0;
				while (process_time[loc][3] != 0)
					loc++;
				process_time[loc][3] = current_process_id;
				process_time[loc][0] = time;
				process_time[loc][1] = time - current_process_arrival_time;	//C.T - A.T
				process_time[loc][2] = process_time[loc][1] - const_burst_time;			//TAT - BT
			}
			else if (current_process_burst_time == qt)
			{
				//note the completion time
				time += qt;
				//finding empty space
				int loc = 0;
				while (process_time[loc][3] != 0)
					loc++;
				process_time[loc][3] = current_process_id;
				process_time[loc][0] = time;
				process_time[loc][1] = time - current_process_arrival_time;	//C.T - A.T
				process_time[loc][2] = process_time[loc][1] - const_burst_time;			//TAT - BT
			}
			else
			{
				time += qt;
				current_process_burst_time -= qt;

				//checking for arrived processes in new time frame
				while (j < total_process && arrival_time[j][1] <= time)	//populating queue with the arrived processes
				{
					q_arrival.push(arrival_time[j][1]);
					q_burst.push(burst_time[j][1]);
					q_id.push(arrival_time[j][0]);
					q_burst_const.push(burst_time[j][2]);
					j++;
				}
				//now pushing back the current process with updated burst time
				q_arrival.push(current_process_arrival_time);
				q_burst.push(current_process_burst_time);
				q_burst_const.push(const_burst_time);
				q_id.push(current_process_id);
			}
		}
		else
			time++;
	}

	cout << "Process ID:\tA.T\tB.T\tC.T\tTAT\tW.T\n";
	for (int i = 0; i < total_process; i++)
	{
		int current_process_id = arrival_time[i][0];
		//finding relavant data for current process id
		int j = 0;
		while (process_time[j][3] != current_process_id)
			j++;
		cout << current_process_id << "\t\t" << arrival_time[i][1] << "\t" << burst_time[i][1] << "\t" << process_time[j][0] << "\t" << process_time[j][1] << "\t" << process_time[j][2] << endl;
	}
	return 0;
}

void ArrangeProcesses(double **arrival_time, int **burst_time, int total_process)
{
	//sorting with respect to arrival time
	for (int i = 0; i < total_process - 1; i++)
	{
		for (int j = i + 1; j < total_process; j++)
		{
			if (arrival_time[i][1] > arrival_time[j][1])
			{
				int temp = arrival_time[i][1];
				arrival_time[i][1] = arrival_time[j][1];
				arrival_time[j][1] = temp;

				temp = burst_time[i][1];
				burst_time[i][1] = burst_time[j][1];
				burst_time[j][1] = temp;

				temp = burst_time[i][2];	//Section not tested
				burst_time[i][2] = burst_time[j][2];
				burst_time[j][2] = temp;

				//exchanging process id
				temp = arrival_time[i][0];
				arrival_time[i][0] = arrival_time[j][0];
				arrival_time[j][0] = temp;

				temp = burst_time[i][0];
				burst_time[i][0] = burst_time[j][0];
				burst_time[j][0] = temp;
			}
		}
	}
}