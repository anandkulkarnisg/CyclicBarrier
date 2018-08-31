#include<iostream>
#include<string>

#include "CyclicBarrier.h"

using namespace std;
using namespace Poco;

// First implement the constructor with default parameter of participants. Here there is no Runnable being run.
CyclicBarrier::CyclicBarrier(const unsigned int& participantCount)
{
	if(participantCount <= 0)
		throw IllegalArgumentException();
	m_parties = participantCount;
	m_count = participantCount;
}

// Implement the constructor with participant count and Runnable item. Here the runnable is present and initialized.
CyclicBarrier::CyclicBarrier(const unsigned int& participantCount, Runnable* runItem)
{
	if(participantCount <= 0)
		throw IllegalArgumentException();
	m_parties = participantCount;
	m_count = participantCount;
	m_isRunnableItem = true;
	m_runnableItem.reset(runItem);
}

// Implement the getParties. This does not require any lock. It returns the number of parties/participants required to trip this barrier.
unsigned int CyclicBarrier::getParties()
{
	return(m_parties);
}

// Implement the isBroken status indicator. This is used to indicate if the barrier is in broken state when used last time. Requires an exclusive Lock while checking this to ensure that 
// Information is accurate.

bool CyclicBarrier::isBroken()
{
	//unique_lock<mutex> exclusiveLock(m_mutex);
	return(m_broken);
}

// Implement the reset method. This is used to reset the barrier to clean or original condition. It is not required unless during last trip it went into broken state.
// While running this it requires an exclusive lock as you want to ensure that no other thread starts/aims to use this barrier while reset is going on.
void CyclicBarrier::reset()
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	breakBarrier();
	nextGeneration();
}

// Implement the getNumberWaiting Method. This is primarily used for debugging purpose. It indicates number of parties currently waiting on the barrier.
// Naturally it requires lock to indicate the number so that it is as accurate as possible.
unsigned int CyclicBarrier::getNumberWaiting()
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	return(m_parties - m_count);
}

// Implement the resetGeneration Method. This simply resets the broken status to false.
void CyclicBarrier::resetGeneration()
{
	m_broken = false;	
} 

// Implement the nextGeneration Method. This method simply resets the counters back to original status so that barrier is ready again to be used.
void CyclicBarrier::nextGeneration()
{	
	m_count = m_parties;
	m_cond.notify_all();
	resetGeneration();
}

// Implement the breakBarrier Method. This method breaks the current barrier.i.e it sets the status of the barrier to the broken and signals other threads.
void CyclicBarrier::breakBarrier()
{
	m_broken = true;
	m_count = m_parties;
	m_cond.notify_all();
} 

// Now Implement the most critical method. Internal private dowait. It covers both the await and await with timeout interface calls. 
unsigned int CyclicBarrier::dowait(const bool& timeOutNeeded, const long& waitTimeMilliSecs)
{
	// First take an exclusive Lock.
	unique_lock<mutex> exclusiveLock(m_mutex);

	// Check if the barrier is broken. If so throw and bow out.
	if(isBroken())
		throw BrokenBarrierException();

	unsigned int returnIndex = --m_count;
	bool runStatus = false;

	// If this is true then we are last thread to trip the barrier.
	if(returnIndex == 0)
	{
		try
		{
			if(m_isRunnableItem)
				(*m_runnableItem.get()).run();
			runStatus = true;
			nextGeneration();
			exclusiveLock.unlock();
			return(0);
		}
		catch(const std::exception& e)
		{
			if(!runStatus)
				breakBarrier();
		}
	}	

	// We are here means we are not the last or tripping thread.
	if(!timeOutNeeded)
		m_cond.wait(exclusiveLock, [&]() { return(this->m_count == m_parties || this->m_broken == true); });
	else
		m_cond.wait_for(exclusiveLock, std::chrono::milliseconds(waitTimeMilliSecs), [&]() { return(this->m_count == m_parties || this->m_broken == true); });

	// We have come here means the condition variable has been signalled and condition met. Let us check various possibilities here.
	if(isBroken())
		throw BrokenBarrierException();

	if(timeOutNeeded && this->m_count != m_parties)
	{
		breakBarrier();
		throw TimeOutException();
	}

	return(returnIndex);
}

// Now implement the await feature. This waits and blocks indefinitely unless one of other threads times out and breaks the barrier or sufficient threads end up tripping the barrier.
unsigned int CyclicBarrier::await()
{
	return(dowait(false,0));
}

// Now implement the await with timeout feature. This times out and breaks the barrier if sufficient participant threads do not arrive in time.
unsigned int CyclicBarrier::await(const long& waitTimeInMilliSecs)
{
	return(dowait(true, waitTimeInMilliSecs));
}

// Implement the default destructor finally. 
CyclicBarrier::~CyclicBarrier()
{
	if(m_isRunnableItem)
		m_runnableItem.reset();
}
