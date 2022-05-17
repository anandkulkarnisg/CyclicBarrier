#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include<chrono>

#include "CyclicBarrier.h"

using namespace std;
using namespace Poco;

class MyWorker : public Runnable
{
	public:
		MyWorker() : Runnable() {}

		virtual void run()
		{
			cout << "This is a poco runnable demo. Must be called only once!!" << endl;
		}
		~MyWorker() { cout << "MyWorker is being released via destructor!" << endl; }
};

CyclicBarrier barrier(5, new MyWorker());
mutex cout_mutex;

void printOut(const string& str1, const string& str2, const thread::id& id)
{
	unique_lock<mutex> lock(cout_mutex);
	cout << str1 << id << str2 << endl;
	lock.unlock();
}

void waitOnBarrier()
{
	try
	{
		int barrierStatus;
		barrierStatus = barrier.await();
		printOut("I am currently in thread id = ", ".My barrier state count is = " + to_string(barrierStatus), this_thread::get_id());
	}
	catch(const exception& e)	
	{
		cout << e.what() << endl;
	} 
}

int main(int argc, char* argv[])
{
	for(int i=0 ; i<10; ++i)
	{	
		thread t1(waitOnBarrier);
		thread t2(waitOnBarrier);
		thread t3(waitOnBarrier);
		thread t4(waitOnBarrier);
		thread t5(waitOnBarrier);

		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();

		this_thread::sleep_for(chrono::seconds(1));	
	}
	return(0);
}
