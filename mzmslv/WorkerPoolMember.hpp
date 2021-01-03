/* ***************************************************************************
 * WorkerPoolMember.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef WORKERPOOLMEMBER_H_
#define WORKERPOOLMEMBER_H_

namespace mzmslv {

class WorkerPool;

/*!
 * The member of a worker pool.
 */
class WorkerPoolMember 
{
public:
	/*!
	 * This pool member's thread.
	 */
	pthread_t m_thread;
	/*!
	 * A pointer to the job currently being done, or NULL. 
	 */
	WorkerPoolJob* m_job;
	/*!
	 * The owning worker pool.
	 */
	WorkerPool* m_pool;
};

} // namespace mzmslv

#endif /*WORKERPOOLMEMBER_H_*/
