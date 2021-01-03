/* ***************************************************************************
 * WorkerPoolClient.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef WORKERPOOLCLIENT_H_
#define WORKERPOOLCLIENT_H_

#include "WorkerPoolJob.hpp"

namespace mzmslv {

/*!
 * An interface which clients of the WorkerPool must implement.
 * The WorkerPool fires the callbacks singlethreaded.
 */
class WorkerPoolClient
{
public:
	/*!
	 * Called by the solver pool to obtain a new problem to solve.
	 * \return a pointer to a new job or NULL if there are no more jobs.
	 * If the WorkerPool is interrupted, it will delete any outstanding
	 * jobs which it doesn't report.
	 */
	virtual WorkerPoolJob* getNextJob() = 0;
	/*!
	 * Called by the worker pool to inform the client that a worker has finished.
	 * \param job the job, which should now be deleted by the client.
	 */
	virtual void jobDone(WorkerPoolJob* job) = 0;
};

} // namespace mzmslv

#endif /*WORKERPOOLCLIENT_H_*/
