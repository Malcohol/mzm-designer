/* ***************************************************************************
 * OfflineSolver.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef OFFLINESOLVER_H_
#define OFFLINESOLVER_H_

#include <map>
#include <mzm/MazezamSolver/MultiMazezamSolver.hpp>
#include <mzm/MzmReaders/MzmReader.hpp>
#include <mzm/RangePred/RangePred.hpp>
#include <mzm/MazezamSolver/Config.hpp>

namespace mzm {

/*!
 * A class which reads a set of mazezams and solves them using a worker pool.
 */
class OfflineSolver : public MultiMazezamSolver
{
public:
	/*!
	 * Constructor.
	 * \param copyMode if true, then solutions to one level are output before the next one is read.
	 */
	OfflineSolver(MzmReader& r, mzmslv::WorkerPool& workerPool, SolutionCollector& collector, unsigned int solutionTypeFlags, bool copyMode);
	/*!
	 * Solve the levels using the worker pool synchronously.
	 */
	void solve();
	
	// WorkerPoolClient interface.
	virtual mzmslv::WorkerPoolJob* getNextJob();
private:
	/*!
	 * The source of the mazezams.
	 */
	MzmReader& m_mzmReader;
	/*!
	 * If in copyMode, then we must output the solutions to one level before
	 * we read the next one. Naturally, this limits concurrency.
	 */
	bool m_copyMode;
private:
	/*!
	 * Set up state for a new level. 
	 */
	void setUpForNextLevel();
};

} // namespace mzm

#endif /*OFFLINESOLVER_H_*/
