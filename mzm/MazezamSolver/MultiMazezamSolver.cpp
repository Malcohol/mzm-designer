/* ***************************************************************************
 * MultiMazezamSolver.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <mzm/MazezamSolver/MultiMazezamSolver.hpp>

#include <cassert>

#include <mzm/MazezamSolver/MazezamSolverJob.hpp>
#include <mzm/MazezamSolver/SolutionCollector.hpp>

namespace mzm {

MultiMazezamSolver::MultiMazezamSolver(mzmslv::WorkerPool& workerPool, SolutionCollector& collector) 
: m_workerPool(workerPool)
, m_collector(collector)
, m_solutionTypeFlags(0)
{
}


void MultiMazezamSolver::jobDone(mzmslv::WorkerPoolJob* job)
{
	assert(m_currentWork.find(job) != m_currentWork.end());
	WorkUnderway underway = m_currentWork[job];
	m_currentWork.erase(job);
	
	MazezamSolutionSource* source;
	
	switch (underway.m_solutionType) {
		case MAZEZAM_SOLUTION_FEWEST_PUSHES: {
			MazezamSolverJobFewestPushes* mazezamJob = static_cast<MazezamSolverJobFewestPushes*>(job);
			if (mazezamJob->getResult() == mzmslv::FOUND_SOLUTION) {
				if (m_solutionTypeFlags & MAZEZAM_SOLUTION_RATING)
					m_collector.collectRating(underway.m_levelNumber, mazezamJob->getRating());
				if (m_solutionTypeFlags & MAZEZAM_SOLUTION_NUM_PUSHES)
					m_collector.collectNumPushes(underway.m_levelNumber, mazezamJob->getPath().size() - 1);
			}
			source = mazezamJob;
			break;
		}
		case MAZEZAM_SOLUTION_FEWEST_MOVES: {
			MazezamSolverJobFewestMoves* mazezamJob = static_cast<MazezamSolverJobFewestMoves*>(job);
			if (mazezamJob->getResult() == mzmslv::FOUND_SOLUTION) {
				if (m_solutionTypeFlags & MAZEZAM_SOLUTION_NUM_MOVES)
					m_collector.collectNumMoves(underway.m_levelNumber, mazezamJob->getPath().size() - 1);
			}
			source = mazezamJob;
			break;
		}
		case MAZEZAM_SOLUTION_FASTEST:
			source = static_cast<MazezamSolverJobFastest*>(job);
			break;
		default:
			assert(false);
	}
	
	if (m_solutionTypeFlags & underway.m_solutionType) {
		m_collector.collectSolution(underway.m_levelNumber, underway.m_solutionType, *source);
	}
}

} // namespace mzm
