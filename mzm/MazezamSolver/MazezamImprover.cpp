/* ***************************************************************************
 * MazezamImprover.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "MazezamImprover.hpp"

#include <cstdlib>
#include <cassert>

#include <mzm/MazezamSolver/MazezamSolverJob.hpp>
#include <mzm/MazezamSolver/SolutionCollector.hpp>
#include <mzm/MazezamSolver/Selection.hpp>

namespace mzm {

MazezamImprover::MazezamImprover (const MazezamData& m, MazezamRating startRating, mzmslv::WorkerPool& workerPool, SolutionCollector& collector, unsigned int radius, const Selection* selection) 
: m_level(m)
, m_collector(collector)
, m_workerPool(workerPool)
, m_selection(selection)
, m_radius(radius)
, m_bestLevel(m)
, m_bestRating(startRating)
{
	// One before the position to start considering.
	m_changes.push_back(-1);
}

MazezamImprover::~MazezamImprover ()
{
}

MazezamData* MazezamImprover::getNeighbourLevel() const
{
	MazezamData* newMazezam = new MazezamData(m_level);
	const int numChanges = m_changes.size();
	for (int i = 0; i < numChanges; ++i) {
		assert(m_changes[i] != indexOfEntrance());
		const div_t coords = div(m_changes[i], m_level.getWidth());
		newMazezam->setBlock(coords.quot, coords.rem, !newMazezam->getBlock(coords.quot, coords.rem));
	}
	return newMazezam;
}

int MazezamImprover::indexOfEntrance() const
{
	return m_level.getStart() * m_level.getWidth();
}

int MazezamImprover::getNextIndex(int current) const
{
	++current;
	if (m_selection) {
		div_t coords = div(current, m_level.getWidth());
		while ((coords.quot < m_level.getHeight()) && (!m_selection->isSelected(coords.quot, coords.rem) || (current == indexOfEntrance()))) {
			++current;
			coords = div(current, m_level.getWidth());
		}
	} else if (current == indexOfEntrance())
		++current;
	return current;
}

bool MazezamImprover::advanceIndex(unsigned int pos)
{
	int current = getNextIndex(m_changes[pos]);	
	// The changes list must be strictly increasing.
	if ((pos + 1 < m_changes.size()) && (current >= m_changes[pos + 1]))
		return false;
	// Changes must lie within the level.
	if (current >= m_level.getWidth() * m_level.getHeight())
		return false;
	// Current is a valid index for this position.
	assert(!m_selection || m_selection->isSelected(current/m_level.getWidth(), current%m_level.getWidth()));
	m_changes[pos] = current;
	return true;
}


void MazezamImprover::resetIndicesToPos(unsigned int pos)
{
	int current = -1;
	for (unsigned int i = 0; i <= pos; ++i)
		m_changes[i] = getNextIndex(current);
}


bool MazezamImprover::advanceNeighbour()
{
	const unsigned int currentSize = m_changes.size();
	assert(currentSize <= m_radius);
	
	// Try to update one of the indices.
	for (unsigned int pos = 0; pos < currentSize; ++pos) {
		if (advanceIndex(pos)) {
			if (pos > 0)
				resetIndicesToPos(pos - 1);
			return true;
		}
	}
	// Otherwise, try to add a new position.
	if (currentSize < m_radius) {
		m_changes.push_back(-1);
		resetIndicesToPos(currentSize);
		return true;
	}
	return false;
}

bool MazezamImprover::improve()
{
	MazezamRating startRating = m_bestRating;
	// Find the best neighbour
	m_workerPool.workSynchronous(this);
	// If it's better than the provided mazezam.
	return (m_bestRating > startRating);
}

void MazezamImprover::improveAsynchronously()
{
	m_workerPool.workAsynchronous(this);
}

mzmslv::WorkerPoolJob* MazezamImprover::getNextJob()
{
	if (advanceNeighbour()) {
		MazezamData* nextLevel = getNeighbourLevel();
		return new MazezamSolverJobFewestPushes(*nextLevel);
		delete nextLevel;
	} else
		return NULL;
}

void MazezamImprover::jobDone(mzmslv::WorkerPoolJob* job)
{
	MazezamSolverJobFewestPushes* solverJob = static_cast<MazezamSolverJobFewestPushes*>(job);
	MazezamRating rating = solverJob->getRating();
	if ((solverJob->getResult() == mzmslv::FOUND_SOLUTION) && (rating > m_bestRating)) {
		m_collector.collectRating(0, rating);
		m_collector.collectNumPushes(0, solverJob->getPath().size() - 1); 
		m_collector.collectSolution(0, MAZEZAM_SOLUTION_FEWEST_PUSHES, *solverJob);
		m_bestLevel = solverJob->getLevel();
		m_collector.collectImprovement(0, m_bestLevel);
		m_bestRating = rating;
	}
}

const MazezamData& MazezamImprover::getBestLevel() const
{
	return m_bestLevel;
}

void MazezamImprover::stop()
{
	m_workerPool.releaseAsynchronous();
}

bool MazezamImprover::isFinished() const
{
	return m_workerPool.isFinished();
}

} // namespace mzm
