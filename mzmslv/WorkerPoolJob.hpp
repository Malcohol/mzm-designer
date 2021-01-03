/* ***************************************************************************
 * WorkerPoolJob.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef WORKERPOOLJOB_H_
#define WORKERPOOLJOB_H_

namespace mzmslv {

/*!
 * This is the unit of work. WorkerPoolClients must create one of these when requested
 */
class WorkerPoolJob
{
public:
	enum Outcome {
		JOB_FINISHED,
		JOB_INTERRUPTED
	};

	/*!
	 * Do the job and return the outcome.
	 */
	virtual Outcome doJob() = 0;
	
	/*!
	 * Should signal the worker doing the job that it should stop as
	 * soon as possible and return JOB_INTERRUPTED.
	 */
	virtual void stop() = 0;
	
	/*!
	 * Virtual destructor.
	 */
	virtual ~WorkerPoolJob() {}
};

} // namespace mzmslv

#endif /*WORKERPOOLJOB_H_*/
