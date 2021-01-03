/* ***************************************************************************
 * MazezamPerson.inl
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

namespace mzm {

mzm_coord MazezamPerson::getX() const {
	return m_x;
}

mzm_coord MazezamPerson::getY() const {
	return m_y;
}

bool MazezamPerson::canMoveNorth() const {
	return !atEntrance() && !atExit() && (m_y > 0) && !getBlock(m_y-1,m_x);
}

bool MazezamPerson::canMoveSouth() const {
	return !atEntrance() && !atExit() && (m_y < getHeight() - 1) && !getBlock(m_y+1,m_x);
}

bool MazezamPerson::canMoveEast() const {
	return ((m_x < getWidth() - 1) && !getBlock(m_y,m_x + 1)) || ((m_y == getFinish()) && (m_x == getWidth() - 1));
}

bool MazezamPerson::canMoveWest() const {
	return ((m_x > 0) && !getBlock(m_y,m_x - 1)) || ((m_y == getStart()) && (m_x == 0));
}

bool MazezamPerson::canPushEast() const {
	return (m_x < getWidth() - 1) && !getBlock(m_y,getWidth() - 1) && getBlock(m_y,m_x + 1);
}

bool MazezamPerson::canPushWest() const {
	return (m_x > 0) && !getBlock(m_y,0) && getBlock(m_y,m_x - 1);
}

bool MazezamPerson::atEntrance() const
{
	return (m_x == -1) && (m_y == getStart());
}

bool MazezamPerson::atExit() const
{
	return (m_x == getWidth()) && (m_y == getFinish());	
}

} // namespace mzm
