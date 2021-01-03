/* ***************************************************************************
 * Config.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <cassert>

#include <mzm/MazezamSolver/Config.hpp>

namespace mzm {

Config::Config(const MazezamData& l) :
	m_level(l), m_xx(m_level.getWidth() - 1), m_y(m_level.getStart())
{
	m_inset = new mzm_coord[m_level.getHeight()];
	for (int i = 0; i < m_level.getHeight(); ++i)
		m_inset[i] = m_level.getInset(i);
}

Config::Config(const Config& other) :
	m_level(other.m_level), m_xx(other.m_xx), m_y(other.m_y)
{
	m_inset = new mzm_coord[m_level.getHeight()];
	for (int i = 0; i < m_level.getHeight(); ++i)
		m_inset[i] = other.m_inset[i];
}

Config& Config::operator=(const Config& other)
{
	if (&other != this) {
		assert (other.m_level.getHeight() == m_level.getHeight());
		m_xx = other.m_xx;
		m_y = other.m_y;
		for (int i = 0; i < m_level.getHeight(); ++i)
			m_inset[i] = other.m_inset[i];
	}
	return *this;
}

Config::Config(const MazezamData& l, const mzm_coord* inset, mzm_coord xx, mzm_coord y) :
	m_level(l), m_xx(xx), m_y(y)
{
	assert(xx < m_level.getWidth());
	assert(y < m_level.getHeight());
	assert(l.checkInsets(inset));
	// there should be no block under the player.
	assert(!((m_level.getRow(y) >> inset[y]) & (1 << xx)));
	m_inset = new mzm_coord[m_level.getHeight()];
	for (int i = 0; i < m_level.getHeight(); ++i)
		m_inset[i] = inset[i];
}

Config::Config(const MazezamData& l, const mzm_coord* inset, mzm_coord xx, mzm_coord y, bool wasLeft) :
	m_level(l), m_xx(xx), m_y(y)
{
	assert(xx < m_level.getWidth());
	assert(y < m_level.getHeight());
	assert(l.checkInsets(inset));
	// The player should be standing on a block
	assert((m_level.getRow(y) >> inset[y]) & (1 << xx));
	// If the push was to the right, then there should be a space on the left of the player.
	assert (wasLeft || !((m_level.getRow(y) >> inset[y]) & (1 << (xx + 1))));
	// If the push was to the left, then there should be a space on the right of the player.
	assert (!wasLeft || !((m_level.getRow(y) >> inset[y]) & (1 << (xx - 1))));
	m_inset = new mzm_coord[m_level.getHeight()];
	for (int i = 0; i < m_level.getHeight(); ++i)
		m_inset[i] = inset[i];
	if (wasLeft)
		--m_inset[y];
	else
		++m_inset[y]; 
}

Config::~Config()
{
	delete[] m_inset;
}

bool Config::operator< (const Config& other) const
{
	if (m_xx != other.m_xx)
		return m_xx > other.m_xx;
	if (m_y != other.m_y)
		return m_y < other.m_y;
	for (int i = 0; i < m_level.getHeight(); ++i) {
		if (m_inset[i] != other.m_inset[i])
			return m_inset[i] < other.m_inset[i];
	}
	// they're equal.
	return false;
}

void Config::getNeighbours(std::vector<Config*>& v) {
	// east
	if (m_xx > 0) {
		// Opportunities to push east
		if (row(m_y) & (1 << (m_xx - 1))) {
			if (!(row(m_y) & 1))
				v.push_back(new Config(m_level,m_inset,m_xx-1,m_y,false));
		} else 
		// Opportunities to move right.
			v.push_back(new Config(m_level,m_inset,m_xx-1,m_y));
	}
	// Opportunities to move North.
	if ((m_y > 0) && !(row(m_y-1) & (1 << m_xx)))
		v.push_back(new Config(m_level,m_inset,m_xx,m_y-1));
	// Opportunities to move South.
	if ((m_y < m_level.getHeight() - 1) && !(row(m_y+1) & (1 << m_xx)))
		v.push_back(new Config(m_level,m_inset,m_xx,m_y+1));
	// West
	if (m_xx < m_level.getWidth() - 1) {
		// Opportunities to push West
		if (row(m_y) & (1 << (m_xx + 1))) {
			if (m_inset[m_y] > 0)
				v.push_back(new Config(m_level,m_inset,m_xx+1,m_y,true));
		} else
		// Opportunities to move West.
			v.push_back(new Config(m_level,m_inset,m_xx+1,m_y));
	}
}

void Config::addInitialMoves(mzm_path& p, mzm_coord& x, mzm_coord& y) const
{
	p.push_back(DIR_EAST);
  	x = 0;
  	y = m_level.getStart();
}	

void Config::addMoves(Config& other, mzm_path& p, mzm_coord& x, mzm_coord& y) const
{
	if (other.m_xx == m_xx - 1) {
		if (canPushEast())
			p.push_back(DIR_PUSH_EAST);
		else if (canMoveEast())
			p.push_back(DIR_EAST);
		else
			throw NoSuchMoves();
		--x;
	}
	else if (other.m_xx == m_xx + 1) {
		if (canPushWest())
			p.push_back(DIR_PUSH_WEST);
		else if (canMoveWest())
			p.push_back(DIR_WEST);
		else
			throw NoSuchMoves();
		++x;
	}
	else if (other.m_y == m_y - 1 && canMoveNorth()) {
		p.push_back(DIR_NORTH);
		--y;
	}
	else if (other.m_y == m_y + 1 && canMoveSouth()) {
		p.push_back(DIR_SOUTH);
		++y;
	}
	else
		throw NoSuchMoves();
}
 
void Config::addFinalMoves(mzm_path& p, mzm_coord& x, mzm_coord& y) const 
{
	p.push_back(DIR_EAST);
}

std::ostream& operator<< (std::ostream& os, Config& m)
{
	// north wall
	for (int i = 0; i < m.m_level.getWidth() + 2; ++i)
		os << CHAR_WALL;
	os << std::endl;
	for (int j = 0; j < m.m_level.getHeight(); ++j) {
		// west wall
		if (j == m.m_level.getStart())
			os << CHAR_ENTRANCE;
		else
			os << CHAR_WALL;
		
		// print rows and zones.
		int b = 1 << (m.m_level.getWidth() - 1);
		int x = 0;
		while (b > 0) {
			if (m.row(j) & b)
				os << CHAR_BLOCK;
			else if (m.m_y == j && m.m_xx == m.m_level.getWidth() - x - 1)
				os << CHAR_PERSON;
			else
				os << CHAR_SPACE;
			b /= 2;
			++x;
		}
		
		// east wall
		if (j == m.m_level.getFinish())
			os << CHAR_EXIT;
		else
			os << CHAR_WALL;
		os << std::endl;
	}
	// south wall
	for (int i = 0; i < m.m_level.getWidth() + 2; i++)
		os << CHAR_WALL;
	os << std::endl;
	return os;
}

} // namespace mzm
