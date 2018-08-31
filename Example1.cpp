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

void printOut(const std::string& str1, const std::string& str2, const std::thread::id& id)
{
	std::unique_lock<std::mutex> lock(cout_mutex);
	std::cout << str1 << id << str2 << std::endl;
	lock.unlock();
}

void waitOnBarrier()
{
	try
	{
		int returnCount = barrier.await();
		printOut("I am currently in thread id = ", ".My barrier state count is = " + std::to_string(returnCount), std::this_thread::get_id());
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

int main(int argc, char* argv[])
{
	int i=0;
	while(i<demoLoopCount)	
	{		
		// Implement a basic testing demo for the barrier with two threads.
		std::thread t1(&waitOnBarrier);

		// Wait for around 5 seconds and then kick of the other thread.
		std::thread t2(&waitOnBarrier);

		t1.join();
		t2.join();
		++i;
	}

	return(0);
}
