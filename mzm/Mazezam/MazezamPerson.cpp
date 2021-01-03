/* ***************************************************************************
 * MazezamPerson.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <mzm/Mazezam/MazezamPerson.hpp>
#include <cassert>

#include <mzmslv/Solver.hpp>
#include <mzm/MazezamSolver/ConfigWalk.hpp>

namespace mzm {

MazezamPerson::MazezamPerson (mzm_coord h, mzm_coord w, mzm_coord s, mzm_coord f)
: MazezamData(h, w, s, f)
, m_y(s)
, m_x(-1)
{
}

MazezamPerson::MazezamPerson (const MazezamPerson& other) 
: MazezamData(other)
, m_y(other.m_y)
, m_x(other.m_x)
{
}

MazezamPerson::MazezamPerson (const MazezamData& m)
: MazezamData(m)
, m_y(m.getStart())
, m_x(-1)
{
}

MazezamPerson& MazezamPerson::operator= (const MazezamPerson& other)
{
	if (&other != this) {
		MazezamData::operator=(other);
		m_y = other.m_y;
		m_x = other.m_x;
	}
	return *this;
}

void MazezamPerson::moveNorth() 
{
	assert(canMoveNorth());
	--m_y;
}

void MazezamPerson::moveSouth()
{
	assert(canMoveSouth());
	++m_y;
}

void MazezamPerson::moveEast()
{
	assert(canMoveEast());
	++m_x;
}

void MazezamPerson::moveWest()
{
	assert(canMoveWest());
	--m_x;
}

void MazezamPerson::pushEast()
{
	assert(canPushEast());
	++m_x;
	shiftEast(m_y);
}

void MazezamPerson::pushWest()
{
	assert(canPushWest());
	--m_x;
	shiftWest(m_y);
}


void MazezamPerson::moveInDirection(mzm_direction d)
{
	switch (d) {
		case DIR_NORTH:
			assert (canMoveNorth());
			moveNorth();
			break;
		case DIR_SOUTH:
			assert (canMoveSouth());
			moveSouth();
			break;
		case DIR_EAST:
			assert (canMoveEast());
			moveEast();
			break;
		case DIR_PUSH_EAST: 
			assert (canPushEast());
			pushEast();
			break;
		case DIR_WEST:
			assert (canMoveWest());
			moveWest();
			break;
		case DIR_PUSH_WEST:
			assert (canPushWest());
			pushWest();
			break;
	}
}

bool MazezamPerson::findPath(mzm_coord y, mzm_coord x, mzm_path& p) const
{
	assert (y >= 0);
	assert (y < getHeight());
	assert ((x >= 0) || ((x == -1) && (y == getStart()))); 
	assert ((x < getWidth()) || ((x == getWidth()) && (y == getFinish())));
	// The MazezamSolver doesn't account for the final position:
	const mzm_coord targetX = (x == getWidth()) ? (x - 1) : x;
	// The MazezamSolver doesn't account for the start position:
	mzm_coord currentX = m_x;
	if (x == -1) {
		currentX = 0;
		p.push_back(DIR_EAST);
	}
	mzm_coord currentY = m_y;
	// We use a breadth-first search to find the shortest way of walking to the target.
	mzmslv::Solver<ConfigWalk> s;
	mzmslv::Solver<ConfigWalk>::path cp;
	ConfigWalk* init = new ConfigWalk(*this, currentX, currentY, targetX, y);
	mzmslv::SolverResult b = s.findSolutionBreadthFirst(init, cp);
	if (b == mzmslv::FOUND_SOLUTION) {
		// add the moves corresponding to the walk.
		for(unsigned int i = 0; i < cp.size() - 1; ++i)
			cp[i]->addMoves(*cp[i+1], p, currentX, currentY);
	}
	// delete the configurations in the solution path.
	mzmslv::Solver<ConfigWalk>::deletePath(cp);
	// It is an error if we cannot find such a walk.
	if (b != mzmslv::FOUND_SOLUTION)
		return false;
	// We were searching for the finishing position.
	if (x == getWidth())
		p.push_back(DIR_EAST);
	return true;
}

void MazezamPerson::followPath(const mzm_path& p, int numSteps)
{
	assert(p.size() >= 0);
	unsigned int destination = numSteps >= 0 ? numSteps : p.size();
	for (unsigned int i = 0; i < destination; ++i)
		moveInDirection(p[i]);
}

} // namespace mzm