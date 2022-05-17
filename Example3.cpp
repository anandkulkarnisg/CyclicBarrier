#include<iostream>
#include<string>
#include<thread>
#include<chrono>
#include<mutex>

#include "CyclicBarrier.h"

using namespace std;

CyclicBarrier barrier(2);
mutex cout_mutex;
const int demoLoopCount = 1000;

void printOut(const string& str1, const string& str2, const thread::id& id)
{
	unique_lock<mutex> lock(cout_mutex);
	cout << str1 << id << str2 << endl;
	lock.unlock();
}

void waitOnBarrier(const bool& waitStatus)
{
	int returnCount;
	try
	{
		if(waitStatus)
		{
			returnCount = barrier.await(5);
			printOut("I am currently in wait with timeout thread id = ", ".My barrier state count is = " + to_string(returnCount), this_thread::get_id());
		}
		else
		{
			this_thread::sleep_for(chrono::milliseconds(3));
			returnCount = barrier.await();	
			printOut("I am currently in wait thread id = ", ".My barrier state count is = " + to_string(returnCount), this_thread::get_id());
		}
	}
	catch(const exception& e)
	{
		cout << "I am at demoLoopCount = " << demoLoopCount << " in thread id = " << this_thread::get_id()  << endl;
		cout << e.what() << endl;
		barrier.reset();
	}
}

int main(int argc, char* argv[])
{
	int i=0;
	while(i<demoLoopCount)
	{		
		// Implement a basic testing demo for the barrier with two threads.
		thread t1(&waitOnBarrier, true);

		// Wait for around 5 seconds and then kick of the other thread.
		thread t2(&waitOnBarrier, false);

		t1.join();
		t2.join();
		++i;
	}

	return(0);
}
