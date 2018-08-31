#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include<chrono>
#include<algorithm>
#include<vector>
#include<condition_variable>
#include<vector>

#include "CyclicBarrier.h"

using namespace std;

CyclicBarrier barrier(5);
std::mutex cout_mutex;
std::condition_variable m_cond;
bool kickStart = false;
const int threadSize = 500;
const long waitTime = 4;
std::vector<int> m_vec;

void printOut(const std::string& str1, const std::string& str2, const std::thread::id& id, const int& barrierCount)
{
	std::unique_lock<std::mutex> lock(cout_mutex);
	std::cout << str1 << id << str2 << std::endl;
	++m_vec[barrierCount];
	lock.unlock();
}

void waitOnBarrier()
{
	try
	{
		while(!kickStart)	{ }
		int barrierStatus = barrier.await();
		printOut("I am currently in thread id = ", ".My barrier state count is = " + std::to_string(barrierStatus), std::this_thread::get_id(), barrierStatus);
	}
	catch(const std::exception& e)	
	{
		std::unique_lock<std::mutex> lock(cout_mutex);
		std::cout << e.what() << std::endl;
		lock.unlock();
	} 
}

int main(int argc, char* argv[])
{
	std::vector<std::thread> v;
	v.reserve(threadSize);
	m_vec.reserve(5);
	m_vec = { 0, 0, 0, 0, 0 };

	for(unsigned int i=0; i<threadSize; ++i)
		v.push_back(std::thread(&waitOnBarrier));

	std::cout << "Main Thread starting a wait for 10 secs before giving go ahead to the barrier await threads" << std::endl;	
	std::this_thread::sleep_for(std::chrono::seconds(waitTime));	
	std::cout << "Setting the kickStart to true now!!" << std::endl;
	kickStart=true;

	std::for_each(v.begin(),v.end(),[&](std::thread& t){ t.join(); });

	// We can verify that all the different counts of barrier from 0 to 4 must be same amount. In 100 tries each type of count must be hit exactly 20 times. Since barrier clears in batches of 5.
	// Each time 5 threads are cleared with different counts from 0 to 4.

	for(unsigned int i=0; i<5; ++i)
		std::cout << "The count of the barrier hits at value = " << i << " is = " << m_vec[i] << std::endl;

	return(0);
}
