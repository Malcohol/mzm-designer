/* ***************************************************************************
 * WorkerPool.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "WorkerPool.hpp"

#include <cassert>

namespace mzmslv {

WorkerPool::WorkerPool(int numThreads)
: m_numPoolMembers(numThreads)
, m_numMembersWorking(0)
, m_state(WAITING_FOR_CLIENT)
, m_client(NULL)
{
	// Allocate memory for the threads.
	m_poolMembers = new WorkerPoolMember[numThreads];
	
	// Initialize the mutex and the barriers.
	pthread_mutex_init(&m_clientLock,NULL);
	pthread_barrier_init(&m_startBarrier, NULL, m_numPoolMembers + 1);
	pthread_barrier_init(&m_endBarrier, NULL, m_numPoolMembers + 1);
	
	// initialize the solver threads.
	for (int i = 0; i < numThreads; ++i) {
		m_poolMembers[i].m_pool = this;
		m_poolMembers[i].m_job = NULL;
		pthread_create(&m_poolMembers[i].m_thread, NULL, threadStartPoint, static_cast<void*>(&m_poolMembers[i]));
	}
}


WorkerPool::~WorkerPool()
{
	releaseAsynchronous();
	m_state = STOPPING;
	
	// Wait for the threads to resynchronize.
 	pthread_barrier_wait(&m_startBarrier);
 	
	// Wait for the solver threads to finish.
	for (int i = 0; i < m_numPoolMembers; ++i) {
		pthread_join(m_poolMembers[i].m_thread, NULL);
	}
	
	delete[] m_poolMembers;
}


void* WorkerPool::threadStartPoint(void* arg)
{
	WorkerPoolMember* a = static_cast<WorkerPoolMember*>(arg);
	WorkerPool *const pool = a->m_pool;
	// Wait until there's a new set of problems or the object is being destroyed.
	pthread_barrier_wait(&pool->m_startBarrier);
	// Keep working until we're told to stop.
	while (pool->m_state != STOPPING) {
		// While there are problems, solve them.
		if (!pool->doAJob(a))
		{
			// Synchronize when the work is finished.
			pthread_barrier_wait(&pool->m_endBarrier);
			// Wait for a new set of problems (or destruction).
			pthread_barrier_wait(&pool->m_startBarrier);
		}	
	}
	return 0;
}

bool WorkerPool::doAJob(WorkerPoolMember* worker)
{
	assert(!worker->m_job);
	pthread_mutex_lock(&m_clientLock);
	// Ask the client for a new job unless it has released the pool
	if ( (m_state == WORKING_SYNCHRONOUSLY) || (m_state == WORKING_ASYNCHRONOUSLY) )
		worker->m_job = m_client->getNextJob();
	pthread_mutex_unlock(&m_clientLock);
	// The return value. Assume that there will be no subsequent job to obtain.
	bool getSubsequentJob = false;
	// Did we get a new problem?
	if (worker->m_job != NULL) {
		if (worker->m_job->doJob() == WorkerPoolJob::JOB_FINISHED) {
			pthread_mutex_lock(&m_clientLock);
			// Inform the client unless the it has released the pool.
			if ( (m_state == WORKING_SYNCHRONOUSLY) || (m_state == WORKING_ASYNCHRONOUSLY) )
			{
				m_client->jobDone(worker->m_job);
				// Since this job was successful, we should try to get another one.
				getSubsequentJob = true;
			}
			pthread_mutex_unlock(&m_clientLock);
		}
#ifndef NDEBUG
		else
			assert(m_state == WAITING_FOR_CLIENT);
#endif
		delete worker->m_job;
		worker->m_job = NULL;
	} else {
		pthread_mutex_lock(&m_clientLock);
		--m_numMembersWorking;
		pthread_mutex_unlock(&m_clientLock);
	}
	return getSubsequentJob;
}


void WorkerPool::workSynchronous(WorkerPoolClient* client)
{
	releaseAsynchronous();
	m_state = WORKING_SYNCHRONOUSLY;
	m_client = client;
	// Start the pool threads.
	pthread_barrier_wait(&m_startBarrier);
	WorkerPoolMember worker;
	worker.m_thread = NULL;
	worker.m_job = NULL;
	worker.m_pool = this;
	while (doAJob(&worker));
	// Wait for the pool threads to finish.
	pthread_barrier_wait(&m_endBarrier);
}


void WorkerPool::workAsynchronous(WorkerPoolClient* client)
{
	if (m_numPoolMembers == 0)
		throw NoPoolMembers();
	releaseAsynchronous();
	m_client = client;
	m_state = WORKING_ASYNCHRONOUSLY;
	m_numMembersWorking = m_numPoolMembers;
	// Start the pool threads.
	pthread_barrier_wait(&m_startBarrier);
}


void WorkerPool::releaseAsynchronous()
{
	if (m_state == WORKING_ASYNCHRONOUSLY) {
		// We lock the client to guarantee that it won't receive subsequent
		// jobDone callbacks, and also to ensure that the call to stop is not
		// called on a deleted object.
		pthread_mutex_lock(&m_clientLock);
		m_state = WAITING_FOR_CLIENT;
		// Tell any working threads not to bother.
		for (int i = 0; i < m_numPoolMembers; ++i)
			if (m_poolMembers[i].m_job) 
				m_poolMembers[i].m_job->stop();
		pthread_mutex_unlock(&m_clientLock);
		// Return the threads to the start state.
		pthread_barrier_wait(&m_endBarrier);
	}
}

void WorkerPool::waitAsynchronous()
{
	if (m_state == WORKING_ASYNCHRONOUSLY) {
		m_state = WAITING_FOR_CLIENT;
		// Wait for the threads to finish.
		pthread_barrier_wait(&m_endBarrier);
	}
}

bool WorkerPool::isFinished() const
{
	return m_numMembersWorking == 0;
}

} // namespace mzmslv
