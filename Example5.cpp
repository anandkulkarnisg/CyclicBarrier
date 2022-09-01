#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include<chrono>

#include "CyclicBarrier.h"

class MyWorker:public Poco::Runnable{
	public:
		MyWorker():Runnable() {}
		virtual void run(){
			std::cout<<"This is a poco runnable demo. Must be called only once!!"<<std::endl;
		}
		~MyWorker(){
      std::cout<<"MyWorker is being released via destructor!"<<std::endl;
    }
};

CyclicBarrier barrier(5, new MyWorker());
std::mutex cout_mutex;

void printOut(const std::string& str1, const std::string& str2, const std::thread::id& id){
	std::unique_lock<std::mutex> lock(cout_mutex);
	std::cout<<str1<<id<<str2<<std::endl;
	lock.unlock();
}

void waitOnBarrier(){
	try{
		int barrierStatus;
		barrierStatus=barrier.await();
		printOut("I am currently in thread id = ", ".My barrier state count is = "+std::to_string(barrierStatus), std::this_thread::get_id());
	}
	catch(const std::exception& e){
		std::cout<<e.what()<<std::endl;
	}
}

int main(int argc, char* argv[]){
	for(int i=0 ; i<10; ++i){	
		std::thread t1(waitOnBarrier);
		std::thread t2(waitOnBarrier);
		std::thread t3(waitOnBarrier);
		std::thread t4(waitOnBarrier);
		std::thread t5(waitOnBarrier);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		std::this_thread::sleep_for(std::chrono::seconds(1));	
	}
	return(0);
}
