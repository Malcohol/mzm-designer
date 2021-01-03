/* ***************************************************************************
 * ConfigWalk.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <mzm/MazezamSolver/ConfigWalk.hpp>

namespace mzm {

ConfigWalk::ConfigWalk(const MazezamData& l, mzm_coord x, mzm_coord y, mzm_coord tx, mzm_coord ty) :
	m_level(l), m_x(x), m_y(y), m_tx(tx), m_ty(ty)
{
}

bool ConfigWalk::operator< (const ConfigWalk& other) const
{
	if (m_x != other.m_x)
		return m_x < other.m_x;
	else
		return m_y < other.m_y;
}


void ConfigWalk::getNeighbours(std::vector<ConfigWalk*>& v) const
{
	mzm_coord xx = m_level.getWidth() - m_x - 1;
	// Opportunities to move right
	if ((m_x < m_level.getWidth() - 1) && !(row(m_y) & (1 << (xx - 1))))
		v.push_back(new ConfigWalk(m_level,m_x+1,m_y,m_tx,m_ty));
	// Opportunities to move up.
	if (m_y > 0 && !(row(m_y-1) & (1 << xx)))
		v.push_back(new ConfigWalk(m_level,m_x,m_y-1,m_tx,m_ty));
	// Opportunities to move down.
	if (m_y < m_level.getHeight() - 1 && !(row(m_y+1) & (1 << xx)))
		v.push_back(new ConfigWalk(m_level,m_x,m_y+1,m_tx,m_ty));
	// Opportunities to move left.
	if ((m_x > 0) && !(row(m_y) & (1 << (xx + 1))))
		v.push_back(new ConfigWalk(m_level,m_x-1,m_y,m_tx,m_ty));
}

void ConfigWalk::addMoves(ConfigWalk& other, mzm_path& p, mzm_coord& x, mzm_coord& y)
{
	if (other.m_x == m_x - 1) {
		p.push_back(DIR_WEST);
		--x;
	}
	else if (other.m_x == m_x + 1) {
		p.push_back(DIR_EAST);
		++x;
	}
	else if (other.m_y == m_y - 1) {
		p.push_back(DIR_NORTH);
		--y;
	}
	else if (other.m_y == m_y + 1) {
		p.push_back(DIR_SOUTH);
		++y;
	}
	else
		throw NoSuchMoves();
} 

} // namespace mzm
