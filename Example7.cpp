#include<iostream>
#include<string>
#include<thread>
#include<vector>
#include<tuple>

#include "CyclicBarrier.h"

using namespace std;

long result=0;
tuple<int, int> var=make_tuple(0, 0);
CyclicBarrier preBarrier(2), postBarrier(2);

void computeDotProduct(const vector<int>& v, const int& threadId)
{
	size_t size=v.size();
	int startCounter=0;
	while(startCounter<v.size())
	{
		int val=v[startCounter];
		int preBrrierCount=preBarrier.await();
		if(threadId)
			get<0>(var)=val;
		else
			get<1>(var)=val;
		int postBarrierCount=postBarrier.await();
		if(!threadId)
			result+=get<0>(var)*get<1>(var);
		++startCounter;
	}
}

long getDotProduct(const vector<int>& v1, const vector<int>& v2)
{
	thread t1(&computeDotProduct, cref(v1), 0);
	thread t2(&computeDotProduct, cref(v2), 1);
	t1.join();
	t2.join();
	return(result);
}

// fb interview questio : Implement a multi threaded sparse array dot product generator. The arrays are coming in from different sources, they are very large.
// They are being read one index at a time from resource to save down on memory. Provide an efficient implementation of dot product. 
// Time Complexity : O(n) , space complexity:O(1)
// solved using Cyclic Barrier.

int main(int argc, char* argv[])
{
	vector<int> v1={0, 1, 2, 1, 0, 2, 5, 2, 1, 5, 2, 1, 0, 0, 0, 1};
	vector<int> v2={1, 0, 5, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0, 1, 1, 1};
	long dotProduct=getDotProduct(v1, v2);
	cout << "dot product = " << dotProduct << endl;
	return(0);
}
