/* ***************************************************************************
 * BackgroundSolver.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "BackgroundSolver.hpp"

#include <cassert>
#include <mzm/MazezamSolver/MazezamSolverJob.hpp>
#include <mzm/MazezamSolver/SolutionCollector.hpp>

namespace mzm {

BackgroundSolver::BackgroundSolver(mzmslv::WorkerPool& workerPool, SolutionCollector& collector) 
: MultiMazezamSolver(workerPool, collector)
, m_level(NULL)
{
}

BackgroundSolver::~BackgroundSolver()
{
	if (m_level != NULL) {
		m_workerPool.releaseAsynchronous();
		delete m_level;
	}
}

mzmslv::WorkerPoolJob* BackgroundSolver::getNextJob()
{
	//assert(!(m_outstandingFlags & ~m_solutionTypeFlags));

	mzmslv::WorkerPoolJob* job = NULL;
	
	if (m_outstandingFlags) {
		MazezamSolutionType type = extractNextSolutionType(m_outstandingFlags);
		job = createMazezamSolverJob(*m_level, type);
		WorkUnderway underway = { 0, type }; 
		m_currentWork[job] = underway;
	}
	
	return job;
}


void BackgroundSolver::setNewLevel(const MazezamData& l, unsigned int solutionFlags)
{
	if (m_level != NULL) {
		m_workerPool.releaseAsynchronous();
		delete m_level;
	}
	m_solutionTypeFlags = solutionFlags;
	m_outstandingFlags = getSearches(m_solutionTypeFlags);
	m_level = new MazezamData(l);
	m_workerPool.workAsynchronous(this);
}


void BackgroundSolver::wait()
{
	m_workerPool.waitAsynchronous();
}

} // namespace mzm