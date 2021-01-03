/* ***************************************************************************
 * MultiMazezamSolver.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MULTIMAZEZAMSOLVER_H_
#define MULTIMAZEZAMSOLVER_H_

#include <map>

#include <mzmslv/WorkerPool.hpp>
#include <mzm/Mazezam/MazezamData.hpp>
#include <mzm/MazezamSolver/MazezamSolutionType.hpp>
#include <mzm/MazezamSolver/MazezamRatingType.hpp>

namespace mzm {

class SolutionCollector;

/*!
 * An abstract class for solving mazezams and passing the solutions
 * to a solution collector. Subclasses must implement the getNextJob
 * method.
 */
class MultiMazezamSolver : public mzmslv::WorkerPoolClient
{
public:
	/*!
	 * Constructor.
	 */
	MultiMazezamSolver(mzmslv::WorkerPool& workerPool, SolutionCollector& collector);
	
	// WorkerPoolClient implementation.
	virtual void jobDone(mzmslv::WorkerPoolJob* job);
	
protected:
	/*!
	 * The solver pool which provides the solver and the solver thread.
	 */
	mzmslv::WorkerPool& m_workerPool;
	/*!
	 * The collector to which this object is providing solutions.
	 */
	SolutionCollector& m_collector;
	/*!
	 * The solutions we should find for this level.
	 */
	unsigned int m_solutionTypeFlags;
	/*!
	 * The solutions we have yet to find for this level.
	 */
	unsigned int m_outstandingFlags;
	/*!
	 * The work being done by a job.
	 */
	struct WorkUnderway {
		int m_levelNumber;
		MazezamSolutionType m_solutionType;
	};
	/*!
	 * The mapping of job pointers to WorkUnderway structures.
	 */
	std::map<mzmslv::WorkerPoolJob*, WorkUnderway> m_currentWork; 
};

} // namespace mzm

#endif /*MULTISOLVER_H_*/
