#include<iostream>
#include<string>
#include<chrono>
#include<condition_variable>
#include<mutex>
#include<Poco/Runnable.h>
#include<memory>

#include "CyclicBarrierExceptions.h"

#ifndef CyclicBarrier_H
#define CyclicBarrier_H

class CyclicBarrier
{
	private:
		bool m_broken = false;					// This is used to track the state of the Barrier. The only cases of Barrier being broken are If one of threads times out and breaks the Barrier due to time out.
		unsigned int m_count;					// This maintains the count of the barrier participants.
		std::mutex m_mutex;						// This is the mutex which holds allows the synchronization to happen.
		std::condition_variable m_cond;			// This is the signalling condition variable to coordinate the threads on wait condition.
		unsigned int m_parties;					// This is used to geenerate the next generation of the Barrier upon successfull finish and auto reset.
		void resetGeneration();					// This resets the barrier to broken set to false which holding lock.
		void nextGeneration();					// This creates the next cycle/generation of the Barrier upon successfull completion or upon reset.
		void breakBarrier();					// This is used to break the barrier and wakeup or signal everyone.
		unsigned int dowait(const bool&, const long&);	// This returns the arrival index upon succes else throws exception due to BarrierBroken or TimeOut. 
		std::unique_ptr<Poco::Runnable> m_runnableItem;	// This adopts and manages the Poco Runnable item passed to the constructor.
		bool m_isRunnableItem = false;			// This is set to indicate if the runnable is passed to the Barrier.

	public:
		CyclicBarrier(const unsigned int&);		// Public Interface of constructor.
		CyclicBarrier(const unsigned int&, Poco::Runnable* runItem);	// Constructor with parameters and runnable item.
		unsigned int getParties();				// This returns the number of parties required to trip this barrier.
		unsigned int await();					// This is await feature with blocking until either broken or successfull.
		unsigned int await(const long&);		// Ths is  await with time out feature. It either returns suceessfull or breaks the barrier upon timeout.
		bool isBroken();						// Get the status of the Barrier. This must be done with Lock.	
		void reset();							// This resets the Barrier to clean initial state after being broken.
		unsigned int getNumberWaiting();		// This returns the number of parties that are currently waiting at the barrier. Done with a Lock.
		~CyclicBarrier();							// Default destructor of the CyclicBarrier class.
};

#endif
