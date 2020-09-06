#include<iostream>
#include<string>
#include<thread>
#include<vector>
#include<tuple>

#include "CyclicBarrier.h"

using namespace std;

void computeDotProduct(const vector<int>& v, const int& threadId, long& result, tuple<int, int>& var, CyclicBarrier& preBarrier, CyclicBarrier& postBarrier)
{
  size_t size=v.size();
  size_t startCounter=0;
  while(startCounter<size)
  {
    int val=v[startCounter];
    preBarrier.await();   // both threads sync here first before computation happens.
    if(threadId)
      get<0>(var)=val;
    else
      get<1>(var)=val;
    postBarrier.await();  // both threads sync here after computation happens.
    if(!threadId)
      result+=get<0>(var)*get<1>(var);  // only the thread 0 does the computation of dot product.
    ++startCounter;
  }
}

long getDotProduct(const vector<int>& v1, const vector<int>& v2)
{
  long result=0;
  tuple<int, int> var=make_tuple(0, 0);
  constexpr short barrierPair=2; // We need a pair of barriers [ i.e two of them ].
  CyclicBarrier preBarrier(barrierPair), postBarrier(barrierPair);
  thread t1(&computeDotProduct, cref(v1), 0, ref(result), ref(var), ref(preBarrier), ref(postBarrier));
  thread t2(&computeDotProduct, cref(v2), 1, ref(result), ref(var), ref(preBarrier), ref(postBarrier));
  t1.join();
  t2.join();
  return(result);
}

// fb interview question : Implement a multi threaded sparse array dot product generator. The arrays are coming in from different sources, they are very large.
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
