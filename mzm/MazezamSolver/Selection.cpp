/* ***************************************************************************
 * Selection.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <mzm/MazezamSolver/Selection.hpp>

#include <cassert>

namespace mzm {

Selection::Selection(mzm_coord height, mzm_coord width)
: m_height(height)
, m_width(width)
{
	assert(height >= MIN_MAZEZAM_HEIGHT);
	assert(height <= MAX_MAZEZAM_HEIGHT);
	assert(width >= MIN_MAZEZAM_WIDTH);
	assert(width <= MAX_MAZEZAM_WIDTH);
	m_row = new mzm_row[height];
	for (int i = 0; i < height; ++i)
		m_row[i] = 0;
}

Selection::~Selection()
{
	delete[] m_row;
}

bool Selection::isSelected(mzm_coord y, mzm_coord x) const
{
	assert(y >= 0);
	assert(y < MAX_MAZEZAM_HEIGHT);
	assert(x >= 0);
	assert(x < MAX_MAZEZAM_WIDTH);
	return m_row[y] & (1 << x);
}

void Selection::select(mzm_coord y, mzm_coord x)
{
	assert(y >= 0);
	assert(y < MAX_MAZEZAM_HEIGHT);
	assert(x >= 0);
	assert(x < MAX_MAZEZAM_WIDTH);
	m_row[y] |= (1 << x);
}

void Selection::unselect(mzm_coord y, mzm_coord x)
{
	assert(y >= 0);
	assert(y < MAX_MAZEZAM_HEIGHT);
	assert(x >= 0);
	assert(x < MAX_MAZEZAM_WIDTH);
	m_row[y] &= ~(1 << x);
}

void Selection::selectAll()
{
	selectRectangle(0, 0, m_height, m_width);
}

void Selection::selectRectangle(mzm_coord y, mzm_coord x, mzm_coord h, mzm_coord w)
{
	assert(y >= 0);
	assert(h >= 0);
	assert(y + h < MAX_MAZEZAM_HEIGHT);
	assert(x >= 0);
	assert(w >= 0);
	assert(x + w < MAX_MAZEZAM_WIDTH);
	for (int i = 0; i < h; ++i)
		m_row[y+i] |= (((1 << (w + 1)) - 1) << x);
}

void Selection::clear()
{
	for (int i = 0; i < m_height; ++i)
		m_row[i] = 0;
}

void Selection::resize(mzm_coord newHeight, mzm_coord newWidth)
{
	assert(newHeight >= MIN_MAZEZAM_HEIGHT);
	assert(newHeight <= MAX_MAZEZAM_HEIGHT);
	assert(newWidth >= MIN_MAZEZAM_WIDTH);
	assert(newWidth <= MAX_MAZEZAM_WIDTH);
	mzm_row* row = m_row;
	if (m_height != newHeight) {
		row = new mzm_row[newHeight];
	}
	for (int i = 0; i < std::min(m_height, newHeight); ++i) {
		row[i] = m_row[i] & ((1 << (std::min(m_width, newWidth))) - 1 );
	}
	if (m_height != newHeight) {
		for (int i = m_height; i < newHeight; ++i)
			row[i] = 0;
		delete[] m_row;
		m_row = row;
		m_height = newHeight;
	}
	m_width = newWidth;
}

bool Selection::hasSelectedPosition() const
{
	for (int i = 0; i < m_height; ++i)
		if (m_row[i])
			return true;
	return false;
}

} // namespace mzm
