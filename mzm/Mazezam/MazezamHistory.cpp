/* ***************************************************************************
 * MazezamHistory.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <mzm/Mazezam/MazezamHistory.hpp>

namespace mzm {

MazezamHistory::MazezamHistory(const MazezamData& m)
: MazezamPerson(m)
, m_iterator(m_path.begin())
{
}

MazezamHistory::MazezamHistory(const MazezamData& m, const mzm_path& p, int index)
: MazezamPerson(m)
, m_path(p)
, m_iterator(m_path.begin())
{
	if (index < 0)
		index = p.size();
	for (int i = 0; i < index; ++i)
		redo();
}

void MazezamHistory::moveNorth() 
{
	MazezamPerson::moveNorth();
	m_path.erase(m_iterator, m_path.end());
	m_path.push_back(DIR_NORTH);
	m_iterator = m_path.end();
}

void MazezamHistory::moveSouth()
{
	MazezamPerson::moveSouth();
	m_path.erase(m_iterator, m_path.end());
	m_path.push_back(DIR_SOUTH);
	m_iterator = m_path.end();
}

void MazezamHistory::moveEast()
{
	MazezamPerson::moveEast();
	m_path.erase(m_iterator, m_path.end());
	m_path.push_back(DIR_EAST);
	m_iterator = m_path.end();
}

void MazezamHistory::moveWest()
{
	MazezamPerson::moveWest();
	m_path.erase(m_iterator, m_path.end());
	m_path.push_back(DIR_WEST);
	m_iterator = m_path.end();
}

void MazezamHistory::pushEast()
{
	MazezamPerson::pushEast();
	m_path.erase(m_iterator, m_path.end());
	m_path.push_back(DIR_PUSH_EAST);
	m_iterator = m_path.end();
}

void MazezamHistory::pushWest()
{
	MazezamPerson::pushWest();
	m_path.erase(m_iterator, m_path.end());
	m_path.push_back(DIR_PUSH_WEST);
	m_iterator = m_path.end();
}

void MazezamHistory::reset()
{
	while (canUndo())
		undo();
}

void MazezamHistory::undo()
{
	assert(canUndo());
	--m_iterator;
	switch (*m_iterator) {
		case DIR_NORTH:
			MazezamPerson::moveSouth();
			break;
		case DIR_SOUTH:
			MazezamPerson::moveNorth();
			break;
		case DIR_EAST:
			MazezamPerson::moveWest();
			break;
		case DIR_WEST:
			MazezamPerson::moveEast();
			break;
		case DIR_PUSH_EAST:
			pullWest();
			break;
		case DIR_PUSH_WEST:
			pullEast();
			break;
		default:
			assert(false);
	}
}

void MazezamHistory::redo()
{
	assert(canRedo());
	switch (*m_iterator) {
		case DIR_NORTH:
			assert (canMoveNorth());
			MazezamPerson::moveNorth();
			break;
		case DIR_SOUTH:
			assert (canMoveSouth());
			MazezamPerson::moveSouth();
			break;
		case DIR_EAST:
			assert (canMoveEast());
			MazezamPerson::moveEast();
			break;
		case DIR_PUSH_EAST: 
			assert (canPushEast());
			MazezamPerson::pushEast();
			break;
		case DIR_WEST:
			assert (canMoveWest());
			MazezamPerson::moveWest();
			break;
		case DIR_PUSH_WEST:
			assert (canPushWest());
			MazezamPerson::pushWest();
			break;
	}
	++m_iterator;
}

bool MazezamHistory::canUndo()
{
	return m_iterator != m_path.begin();
}

bool MazezamHistory::canRedo()
{
	return m_iterator != m_path.end();
}

void MazezamHistory::pullEast()
{
	assert(getX() > 0);
	assert(getBlock(getY(), getX() - 1));
	shiftEast(getY());
	MazezamPerson::moveEast();
}

void MazezamHistory::pullWest()
{
	assert(getX() < getWidth());
	assert(getBlock(getY(), getX() + 1));
	shiftWest(getY());
	MazezamPerson::moveWest();	
}

} // namespace mzm