#include<iostream>
#include<cstdlib>
#include<ctime>
#include "queue.h"
using namespace std;
const int minute_per_hour = 60;
bool newCustomer(double x);
int main()
{
	//set the random seeds
	srand(time(0));
	// enter the max queue length
	cout << "case: snake's bank ATM" << endl;
	cout << "please enter the max number of customers your bank ATM can hold:" << endl;
	int qs;
	cin >> qs;
	queue line(qs);
	//enter the simulation times
	cout << "please enter the simulation hours your system process:" << endl;
	int hr;
	cin >> hr;
	long cycleLimit = hr*minute_per_hour;
	//enter the customers will arrive to your bank every per hour  
	cout << "please enter the customers will arrive to your bank every per hour:" << endl;
	double custPerHr;
	cin >> custPerHr;
	double minPerCust = minute_per_hour / custPerHr;
	//report items
	Item item;          //customer item
	long turnaways = 0; // turned away by full queue
	long customers = 0; // joined the queue
	long served = 0;    // served during the simulation
	long sum_line = 0;  // cumulative line length
	int  wait_time = 0; // time until autoteller is free
	long line_wait = 0; // cumulative time in line
	//simulation process
	for (int cycle = 0; cycle < cycleLimit; cycle++)
	{
		if (newCustomer(minPerCust))
		{
			if (line.isFull())
				turnaways++;
			else
			{
				customers++;
				item.setCustomer(cycle);
				line.addItem(item);
			}
		}
		if (wait_time <= 0 && !line.isEmpty())
		{
			line.dropItem(item);
			wait_time = item.ptime();
			served++;
			line_wait += cycle - item.when();
		}
		if (wait_time > 0)
			wait_time--;
		sum_line += line.queueCount();
	}
//reporting results
	if (customers > 0)
	{
		cout << "customers accepted: " << customers << endl;
		cout << " customers served: " << served << endl;
		cout << " turnaways: " << turnaways << endl;
		cout << "average queue size: ";
		cout.precision(2);
		cout.setf(ios_base::fixed, ios_base::floatfield);
		cout << (double)sum_line / cycleLimit << endl;
		cout << " average wait time: "
			<< (double)line_wait / served << " minutes\n";
	}
	else
    {
        cout << "No customers!\n";
        cout << "Done!\n";
    }
		
    return 0;
}

bool newCustomer(double x)
{
	return rand()*x/RAND_MAX<1;
}
