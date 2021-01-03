/* ***************************************************************************
 * OfflineSolver.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <mzm/MazezamSolver/OfflineSolver.hpp>

#include <cassert>
#include <memory>

#include <mzm/MazezamSolver/MazezamSolverJob.hpp>
#include <mzm/MazezamSolver/SolutionCollector.hpp>

namespace mzm {

OfflineSolver::OfflineSolver(MzmReader& r, mzmslv::WorkerPool& workerPool, SolutionCollector& collector, unsigned int solutionTypeFlags, bool copyMode)
: MultiMazezamSolver(workerPool, collector)
, m_mzmReader(r)
, m_copyMode(copyMode)
{
	m_solutionTypeFlags = solutionTypeFlags;
	m_outstandingFlags = 0;
}


mzmslv::WorkerPoolJob* OfflineSolver::getNextJob()
{
	if ( !m_outstandingFlags ) {
		// Copy mode does all solutions for each level separately.
		if (m_copyMode || !m_mzmReader.next())
			return NULL;
	
		setUpForNextLevel(); 
	}
	
	std::auto_ptr<MazezamData> level(m_mzmReader.getLevel());
	MazezamSolutionType type = extractNextSolutionType(m_outstandingFlags);
	mzmslv::WorkerPoolJob* job = createMazezamSolverJob(*level, type);
	WorkUnderway underway = { m_mzmReader.getLevelNumber(), type }; 
	m_currentWork[job] = underway;
	
	return job;
}


void OfflineSolver::solve()
{
	if (m_copyMode) {
		// Copy mode does work for each level separately.
		while (m_mzmReader.next()) {
			setUpForNextLevel();
			m_workerPool.workSynchronous(this);
			m_mzmReader.copyLevel();
		}
	} else {
		m_workerPool.workSynchronous(this);
	}
}

void OfflineSolver::setUpForNextLevel()
{
	m_outstandingFlags = getSearches(m_solutionTypeFlags);
	m_collector.collectLevelNumber(m_mzmReader.getLevelNumber());	
}

} // namespace mzm
