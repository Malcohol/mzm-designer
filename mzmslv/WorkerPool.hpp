/* ***************************************************************************
 * WorkerPool.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef WORKERPOOL_H_
#define WORKERPOOL_H_

#include <pthread.h>

#include "WorkerPoolClient.hpp"
#include "WorkerPoolMember.hpp"

namespace mzmslv {

/*!
 * A pool of workers, which can do work simulateously.
 * The workerPool is not itself thread safe (i.e. the solverPool should not be used
 * by multiple client threads).
 */
class WorkerPool
{
public:
	/*!
	 * Constructor.
	 * \param numThreads the number of solver threads to create.
	 */
	WorkerPool(int numThreads);
	/*!
	 * Destructor.
	 */
	virtual ~WorkerPool();
	/*!
	 * Do work using the pool threads and the main thread, returning when all the problems
	 * have been solved.
	 * \param client the client which provides jobs and is informed when they are finished.
	 */
	void workSynchronous(WorkerPoolClient* client);
	/*!
	 * The exception thrown if there are no members in the pool.
	 */
	class NoPoolMembers {};
	/*!
	 * Do work using the pool threads, returning immediately.
	 * No support is provided for avoiding subsequent clashes between the solver pool threads
	 * and the other threads using the client.
	 * \param client the client which provides jobs and is informed when they are finished.
	 * \throw NoPoolMembers is there are no pool members.
	 */
	void workAsynchronous(WorkerPoolClient* client);
	/*!
	 * Informs the solver that the client to asynchronous calls is no longer interested
	 * in receiving solutions. Can be safely called even if the pool was not solving
	 * asynchronously. The client will not receive jobDone callbacks after this method
	 * has returned.
	 */
	void releaseAsynchronous();
	/*!
	 * Waits for the solver to finish when it is running asynchronously.
	 */
	void waitAsynchronous();
	/*!
	 * Has an asynchronous solver finished.
	 */
	bool isFinished() const;
private:
	/*!
	 * Members of the pool.
	 * These structures are also used as the argument passed to each thread as its starts.
	 */
	WorkerPoolMember* m_poolMembers;
	/*!
	 * The number of solver threads to use.
	 */
	int m_numPoolMembers;
	/*!
	 * The number of pool members currently doing jobs.
	 */
	int m_numMembersWorking;
	/*!
	 * The thread start point (which must be static). 
	 */
	static void* threadStartPoint(void* arg);
	/*!
 	 * Try to obtain a new problem and solve it.
 	 * \return false if there was no problem to solve.
 	 */
	bool doAJob(WorkerPoolMember* worker);
	/*!
	 * The states the worker pool can be in.
	 */
	enum State {
		WAITING_FOR_CLIENT,
		WORKING_SYNCHRONOUSLY,
		WORKING_ASYNCHRONOUSLY,
		STOPPING
	};
	/*!
	 * The current state of the solver pool.
	 */
	State m_state;
	/*!
	 * A lock which serializes access to the client.
	 */
	pthread_mutex_t m_clientLock;
	/*!
	 * When the threads are waiting for a new client, they wait on this.
	 */
	pthread_barrier_t m_startBarrier;
	/*!
	 * When there is no more work, we resynchronize the threads.
	 */
	pthread_barrier_t m_endBarrier;
	/*!
	 * The client of the solver pool.
	 */
	WorkerPoolClient* m_client;
};

} // namespace mzmslv

#endif /*WORKERPOOL_H_*/
