#include<iostream>
#include<string>
#include<thread>

#include "CyclicBarrier.h"

CyclicBarrier barrier(2);
void threadFunc(const bool& waitMode){
	int localBarrierCount=0;
	if(waitMode){
		try{	
			localBarrierCount=barrier.await(1000, TimeUnit::MicroSeconds);//On the edge of 1000 micro seconds the barrier either can go broke waiting or work perfectly. This is the tolerence edge!!
		}catch(const std::exception& e){
			std::cout<<localBarrierCount<<e.what()<<std::endl;
		}
	}
	else{
		try{
			localBarrierCount = barrier.await();
		}catch(const std::exception& e){
			std::cout<<localBarrierCount<<e.what()<<std::endl;
		}
	}
}
int main(int argc, char* argv[]){
	// First spin off two threads. One simply hit await and the other kicks on with a wait time of zero. This case currently seems to be going into lock down mode.
	// Investigating why this example.
	int i=0;
	while(i<10000){
		std::thread t1(&threadFunc, false);
		std::thread t2(&threadFunc, true);
		t1.join();
		t2.join();
		++i;
	}
	return(0);
}
