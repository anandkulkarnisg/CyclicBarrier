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
mutex cout_mutex;
condition_variable m_cond;
bool kickStart = false;
const int threadSize = 500;
const long waitTime = 4;
vector<int> m_vec;

void printOut(const string& str1, const string& str2, const thread::id& id, const int& barrierCount)
{
	unique_lock<mutex> lock(cout_mutex);
	cout << str1 << id << str2 << endl;
	++m_vec[barrierCount];
	lock.unlock();
}

void waitOnBarrier()
{
	try
	{
		while(!kickStart)	{ }
		int barrierStatus = barrier.await();
		printOut("I am currently in thread id = ", ".My barrier state count is = " + to_string(barrierStatus), this_thread::get_id(), barrierStatus);
	}
	catch(const exception& e)	
	{
		unique_lock<mutex> lock(cout_mutex);
		cout << e.what() << endl;
		lock.unlock();
	} 
}

int main(int argc, char* argv[])
{
	vector<thread> v;
	v.reserve(threadSize);
	m_vec.reserve(5);
	m_vec = { 0, 0, 0, 0, 0 };

	for(unsigned int i=0; i<threadSize; ++i)
		v.push_back(thread(&waitOnBarrier));

	cout << "Main Thread starting a wait for 10 secs before giving go ahead to the barrier await threads" << endl;	
	this_thread::sleep_for(chrono::seconds(waitTime));	
	cout << "Setting the kickStart to true now!!" << endl;
	kickStart=true;

	for_each(v.begin(),v.end(),[&](thread& t){ t.join(); });

	// We can verify that all the different counts of barrier from 0 to 4 must be same amount. In 100 tries each type of count must be hit exactly 20 times. Since barrier clears in batches of 5.
	// Each time 5 threads are cleared with different counts from 0 to 4.

	for(unsigned int i=0; i<5; ++i)
		cout << "The count of the barrier hits at value = " << i << " is = " << m_vec[i] << endl;

	return(0);
}
