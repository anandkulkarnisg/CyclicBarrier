#include<iostream>
#include<string>
#include<exception>

#ifndef CyclicBarrierExceptions_H
#define CyclicBarrierExceptions_H

// This is thrown during consturction due to invalid parameter being passed.
const char* IllegalArgumentException_msg = "Exception : The argument passed to the barrier is invalid. Please enter valid unsigned +ve integer";

struct IllegalArgumentException : public std::exception
{
	const char * what () const throw ()
	{
		return(IllegalArgumentException_msg);
	}
};

// This is thrown due to the Barrier being broken during the await. It happens due to one of the threads timing out on await with timeout feature and setting the barrier to broken state.
const char* BrokenBarrierException_msg = "Exception : The barrier went into broken state while await. One of the waiting party threads timed out.Please investigate.";

struct BrokenBarrierException : public std::exception
{
    const char * what () const throw ()
    {
        return(BrokenBarrierException_msg);
    }
};

// This is thrown due to Timeout on await feature with timeout parameter. It breaks the barrier before throwing this exception out and signalling other threads to notify.
const char* TimeOutException_msg = "Exception : Exiting due to timeout on the wait. Setting the barrier to broken state.Please investigate. To reuse reset the barrier."; 

struct TimeOutException : public std::exception
{
	const char * what () const throw ()
	{
		return(TimeOutException_msg);
	}
};

#endif

