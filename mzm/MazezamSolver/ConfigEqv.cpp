/* ***************************************************************************
 * ConfigEqv.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <cassert>

#include <mzm/MazezamSolver/ConfigEqv.hpp>
#include <mzmslv/Solver.hpp>
#include <mzm/MazezamSolver/ConfigWalk.hpp>

namespace mzm {

ConfigEqv::ConfigEqv(const MazezamData& m) :
	m_level(m)
{
	assert (m.isValid());
	assert (m_level.getStart() < m_level.getHeight());
	assert (m_level.getFinish() < m_level.getHeight());
	
	m_inset = new mzm_coord[m_level.getHeight()];
	try {
		m_zone = new mzm_row[m_level.getHeight()];
		try { // buildZone is recursive, so it may cause an exception.
			for (int i = 0; i < m_level.getHeight(); ++i) {
				m_inset[i] = m_level.getInset(i);
				m_zone[i] = 0;
			}
			buildZone(1 << (m_level.getWidth() - 1), m_level.getStart());
		} catch (...) {
			delete[] m_zone;
			throw;
		}
	} catch (...) {
		delete[] m_inset;
		throw;
	}
}
	
ConfigEqv::ConfigEqv(const MazezamData& l, const mzm_coord* inset, int xxx, mzm_coord y) :
	m_level(l)
{
	assert (y < m_level.getHeight());
	assert (xxx < (1 << m_level.getWidth()));
	assert(l.checkInsets(m_inset));
	assert (!(row(y) & xxx));
	
	m_inset = new mzm_coord[m_level.getHeight()];
	try {
		m_zone = new mzm_row[m_level.getHeight()];
		try { // buildZone is recursive, so it may cause an exception.
			for (int i = 0; i < m_level.getHeight(); ++i) {
				m_inset[i] = inset[i];
				m_zone[i] = 0;
			}
			buildZone(xxx, y);
		} catch (...) {
			delete[] m_zone;
			throw;
		}
	} catch (...) {
		delete[] m_inset;
		throw;
	}
}

ConfigEqv::ConfigEqv(const MazezamData& l, const mzm_coord* inset, mzm_row xxx, mzm_coord y, bool wasLeft) :
	m_level(l)
{
	assert (y < m_level.getHeight());
	assert (xxx < (mzm_row) (1 << m_level.getWidth()));
	assert (l.checkInsets(inset));
	// the player should be standing on a block.
	assert ((m_level.getRow(y) >> inset[y]) & xxx);
	// If the push was to the right, then there should be a space on the left of the player.
	assert (wasLeft || !((m_level.getRow(y) >> inset[y]) & (xxx << 1)));
	// If the push was to the left, then there should be a space on the right of the player.
	assert (!wasLeft || !((m_level.getRow(y) >> inset[y]) & (xxx >> 1)));
	
	m_inset = new mzm_coord[m_level.getHeight()];
	try {
		m_zone = new mzm_row[m_level.getHeight()];
		try { // buildZone is recursive, so it may cause an exception.
			for (int i = 0; i < m_level.getHeight(); ++i) {
				m_inset[i] = inset[i];
				m_zone[i] = 0;
			}
			if (wasLeft)
				--m_inset[y];
			else
				++m_inset[y];
			buildZone(xxx, y);
		} catch (...) {
			delete[] m_zone;
			throw;
		}
	} catch (...) {
		delete[] m_inset;
		throw;
	}
}

ConfigEqv::~ConfigEqv()
{
	delete[] m_inset;
	delete[] m_zone;
}


void ConfigEqv::buildZone(int xxx, mzm_coord y)
{
	// check there's no block in the way
	assert (!(row(y) & xxx));
	if (!(m_zone[y] & xxx)) {
		// add x,y
		m_zone[y] |= xxx;
		// try north
		if ((y > 0) && !(row(y-1) & xxx))
			buildZone(xxx,y-1);
		// try south
		if ((y < m_level.getHeight()-1) && !(row(y+1) & xxx))
			buildZone(xxx,y+1);
		// try east
		if ((xxx > 1) && !(row(y) & (xxx >> 1)))
			buildZone(xxx >> 1,y);
		// try west
		if ((xxx < (1 << (m_level.getWidth()-1))) && !(row(y) & (xxx << 1)))
			buildZone(xxx << 1,y);
	}
}


bool ConfigEqv::operator< (const ConfigEqv& other) const
{
	// This isn't quite strong enough.
	//assert (m_level == other.m_level);
	for (int i = 0; i < m_level.getHeight(); ++i) {
		if (m_inset[i] != other.m_inset[i])
			return m_inset[i] < other.m_inset[i];
		if (m_zone[i] != other.m_zone[i])
			return m_zone[i] < other.m_zone[i];
	}
	// they're equal
	return false;
}


void ConfigEqv::getNeighbours(std::vector<ConfigEqv*>& v) const {
	mzm_row pushes;
	for (int i = 0; i < m_level.getHeight(); ++i) {
		// Opportunities to push right
		if (!(row(i) & 1)) {
			pushes = row(i) & (m_zone[i] >> 1);
			// handle pushes on this row
			mzm_row xxx = 1;
			while (pushes > 0) {
				if (pushes & 1)
					v.push_back(new ConfigEqv(m_level,m_inset,xxx,i,false));
				pushes >>= 1;
				xxx <<= 1;
			}
		}
		// Opportunities to push left
		if (m_inset[i] > 0) {
			pushes = row(i) & (m_zone[i] << 1);
			// handle pushes on this row
			mzm_row xxx = 1;
			while (pushes > 0) {
				if (pushes & 1)
					v.push_back(new ConfigEqv(m_level,m_inset,xxx,i,true));
				pushes >>= 1;
				xxx <<= 1;
			}
		}
	}
}

void ConfigEqv::findPush(ConfigEqv& other, mzm_coord& x, mzm_coord& y, bool& isLeft) const
{
	// find y
	y = 0;
	while ((y < m_level.getHeight()) && (m_inset[y] == other.m_inset[y])) ++y;
	if (y == m_level.getHeight()) throw NoSuchMoves();
	// find x
	mzm_row pushes;
	if (m_inset[y] + 1 == other.m_inset[y]) {
		// The push was right
		assert (!(row(y) & 1));
		isLeft = false;
		pushes = row(y) & (m_zone[y] >> 1);
		x = m_level.getWidth() - 1;
		while (pushes > 0) {
			// x the result of a push right and (x,y) is in other's zone
			if ((pushes & 1) && (other.m_zone[y] & (1 << (m_level.getWidth() - x - 1)))) {
				--x;
				return;
			}
			pushes >>= 1;
			--x;
		}
	} else if (m_inset[y] - 1 == other.m_inset[y]) {
		// the push was left
		assert (m_inset[y] > 0);
		isLeft = true;
		pushes = row(y) & (m_zone[y] << 1);
		x = m_level.getWidth() - 1;
		while (pushes > 0) {
			// x the result of a push left and (x,y) is in other's zone
			if ((pushes & 1) && (other.m_zone[y] & (1 << (m_level.getWidth() - x - 1)))) {
				++x;
				return;
			}
			pushes >>= 1;
			--x;
		}
	} else
		throw NoSuchMoves();
}

void ConfigEqv::addInitialMoves(mzm_path& p, mzm_coord& x, mzm_coord& y) const
{
	p.push_back(DIR_EAST);
  	x = 0;
  	y = m_level.getStart();
}

void ConfigEqv::addMoves(ConfigEqv& other, mzm_path& p, mzm_coord& x, mzm_coord& y) const
{
	// the target x and y coordinates.
	mzm_coord tx, ty;
	bool isLeft;
	findPush(other,tx,ty,isLeft);
	// We use a breadth-first search to find the shortest way of walking to the target.
	mzmslv::Solver<ConfigWalk> s;
	mzmslv::Solver<ConfigWalk>::path cp;
	MazezamData m(m_level, m_inset);
	ConfigWalk* init = new ConfigWalk(m, x, y, tx, ty);
	mzmslv::SolverResult b = s.findSolutionBreadthFirst(init, cp);
	if (b == mzmslv::FOUND_SOLUTION) {
		// add the moves corresponding to the walk.
		for(unsigned int i = 0; i < cp.size() - 1; ++i)
			cp[i]->addMoves(*cp[i+1],p,x,y);
		// add an additional move corresponding to the push.
		if (isLeft) {
			p.push_back(DIR_PUSH_WEST);
			--x;
		}
		else {
			p.push_back(DIR_PUSH_EAST);
			++x;
		}
	}
	// delete the configurations in the solution path.
	for (mzmslv::Solver<ConfigWalk>::path::iterator i = cp.begin(); i != cp.end(); ++i)
		delete *i;
	// It is an error if we cannot find such a walk.
	if (b != mzmslv::FOUND_SOLUTION)
		throw NoSuchMoves();
}


void ConfigEqv::addFinalMoves(mzm_path& p, mzm_coord& x, mzm_coord& y) const
{
	// We use a breadth-first search to find the shortest way of walking to the target.
	mzmslv::Solver<ConfigWalk> s;
	mzmslv::Solver<ConfigWalk>::path cp;
	MazezamData m(m_level, m_inset);
	ConfigWalk* init = new ConfigWalk(m, x, y, m_level.getWidth() - 1, m_level.getFinish());
	mzmslv::SolverResult b = s.findSolutionBreadthFirst(init, cp);
	if (b == mzmslv::FOUND_SOLUTION) {
		// add the moves corresponding to the walk.
		for(unsigned int i = 0; i < cp.size() - 1; ++i)
			cp[i]->addMoves(*cp[i+1],p,x,y);
	}
	// delete the configurations in the solution path.
	for (mzmslv::Solver<ConfigWalk>::path::iterator i = cp.begin(); i != cp.end(); ++i)
		delete *i;
	// It is an error if we cannot find such a walk.
	if (b != mzmslv::FOUND_SOLUTION)
		throw NoSuchMoves();
	p.push_back(DIR_EAST);
}

float ConfigEqv::getEstimatedDistance () const
{
	if (isGoal()) 
		return 0.0;
	else
		return 1.0;
// This is the old heuristic, which is neither admissible nor consistent.
//	mzm_row r = 0;
//	for (int i = 0; i < m_level.height; ++i)
//		r |= row(i);
//	assert (r != 0);
//	mzm_coord c = 0;
//	while (!(r & (1 << c))) ++c;
//	
//	return c;
}

} // namespace mzm