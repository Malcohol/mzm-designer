/* ***************************************************************************
 * BackgroundSolver.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef BACKGROUNDSOLVER_H_
#define BACKGROUNDSOLVER_H_

#include <mzm/MazezamSolver/MultiMazezamSolver.hpp>

namespace mzm {

/*!
 * BackgroundSolvers solve a single level and inform their observer of the result.
 */
class BackgroundSolver : public MultiMazezamSolver
{
public:
	BackgroundSolver(mzmslv::WorkerPool& workerPool, SolutionCollector& collector);
	virtual ~BackgroundSolver();
	
	// WorkerPoolClient implementation.
	virtual mzmslv::WorkerPoolJob* getNextJob();

	/*!
	 * Start solving the new level (interrupts the solver thread if it
	 * is currently working).
	 */
	void setNewLevel(const MazezamData& l, unsigned int solutionFlags);
	/*!
	 * Wait for the solver to finish.
	 */
	void wait();
	
private:
	/*!
	 * The level being solved.
	 */
	MazezamData* m_level; 
};

} // namespace mzm

#endif /*BACKGROUNDSOLVER_H_*/
