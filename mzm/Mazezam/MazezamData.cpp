/* ***************************************************************************
 * MazezamData.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "MazezamData.hpp"

#include <cassert>

namespace mzm {

MazezamData::MazezamData (mzm_coord h, mzm_coord w, mzm_coord s, mzm_coord f)
: m_height(h)
, m_width(w)
, m_start(s)
, m_finish(f)
{
	assert(m_height >= MIN_MAZEZAM_HEIGHT);
	assert(m_width >= MIN_MAZEZAM_WIDTH);
	assert(m_height <= MAX_MAZEZAM_HEIGHT);
	assert(m_width <= MAX_MAZEZAM_WIDTH);
	assert((m_start >= 0) && (m_start < m_height));
	assert((m_finish >= 0) && (m_finish < m_height));
	m_row = new mzm_row[h];
	try {
		m_inset = new mzm_coord[h];
		for (int j = 0; j < h; ++j) {
			m_row[j] = 0;
			m_inset[j] = w;
		}
	} catch (...) {
		delete[] m_row;
		throw;
	}
}

MazezamData::MazezamData(const MazezamData& other, const mzm_coord* i)
: m_height(other.m_height)
, m_width(other.m_width)
, m_start(other.m_start)
, m_finish(other.m_finish)
{
	m_row = new mzm_row[m_height];
	try {
		m_inset = new mzm_coord[m_height];
		for (int j = 0; j < m_height; ++j) {
			m_row[j] = other.m_row[j];
			m_inset[j] = i[j];
		}
	} catch (...) {
		delete[] m_row;
		throw;
	}
	assert(isValid());
}

MazezamData::MazezamData(array_type& block, mzm_coord h, mzm_coord w, mzm_coord s, mzm_coord f)
: m_height(h)
, m_width(w)
, m_start(s)
, m_finish(f)
{
	assert(m_height >= MIN_MAZEZAM_HEIGHT);
	assert(m_width >= MIN_MAZEZAM_WIDTH);
	assert(m_height <= MAX_MAZEZAM_HEIGHT);
	assert(m_width <= MAX_MAZEZAM_WIDTH);
	assert((m_start >= 0) && (m_start < m_height));
	assert((m_finish >= 0) && (m_finish < m_height));
	m_row = new mzm_row[m_height];
	try {
		m_inset = new mzm_coord[m_height];
		bool found;
		
		for (int y = 0; y < m_height; ++y) {
			m_row[y] = 0;
			m_inset[y] = 0;
			found = false;
			for (int x = 0; x < m_width; ++x) {
				if (block[y][x]) {
					m_row[y] |= (1 << (m_width - x - 1));
					found = true;
				}
				else if (!found) {
					++m_inset[y];
				}
			}
			m_row[y] <<= m_inset[y];
		}
	} catch (...) {
		delete [] m_row;
		throw;
	}
	assert(isValid());
}

MazezamData::MazezamData(const MazezamData& other)
: m_height(other.m_height)
, m_width(other.m_width)
, m_start(other.m_start)
, m_finish(other.m_finish)
{
	m_row = new mzm_row[m_height];
	try {
		m_inset = new mzm_coord[m_height];
		for (int j = 0; j < m_height; ++j) {
			m_row[j] = other.m_row[j];
			m_inset[j] = other.m_inset[j];
		}
	} catch (...) {
		delete[] m_row;
		throw;
	}
}

MazezamData& MazezamData::operator= (const MazezamData& other)
{
	if (&other != this) {
		m_height = other.m_height;
		m_width = other.m_width;
		m_start = other.m_start;
		m_finish = other.m_finish;
		if (m_height != other.m_height) {
			delete[] m_row;
			delete[] m_inset;
			// TODO - if these throw an exception, then the object will be in an
			// invalid state.
			m_row = new mzm_row[m_height];
			m_inset = new mzm_coord[m_height];					
		}
		for (int j = 0; j < m_height; ++j) {
			m_row[j] = other.m_row[j];
			m_inset[j] = other.m_inset[j];
		}
	}
	return *this;
}

MazezamData::~MazezamData()
{
	delete[] m_row;
	delete[] m_inset;
}

bool MazezamData::canSetDimensions(mzm_coord h, mzm_coord w) const
{
	assert(h >= MIN_MAZEZAM_HEIGHT);
	assert(w >= MIN_MAZEZAM_WIDTH);
	assert(h <= MAX_MAZEZAM_HEIGHT);
	assert(w <= MAX_MAZEZAM_WIDTH);
	
	if ((h == m_height) && (w == m_width))
		return true;
		
	if ((h == m_height) && (w == m_width))
		return false;
	// Don't allow the start or finish row to be deleted.
	if ((h <= m_start) || (h <= m_finish))
		return false;
	// Don't allow blocks to be deleted.
	for (int y = h; y < m_height; ++y)
		if (m_row[y])
			return false;
	if (w < m_width) {
		for (int y = 0; y < h; ++y)
			if ((m_row[y] >> m_inset[y]) & ((1 << (m_width - w)) - 1))
				return false;
	}
	return true;
}

void MazezamData::setDimensions(mzm_coord h, mzm_coord w)
{
	assert(canSetDimensions(h, w));
	// We use these pointers for the new row and inset data.
	mzm_row* row = m_row;
	mzm_coord* inset = m_inset;
	if (m_height != h) {
		// TODO - if these throw an exception, then the object will be in an
		// invalid state.
		row = new mzm_row[h];
		inset = new mzm_coord[h];
	} 
	for (int i = 0; (i < m_height) && (i < h); ++i) {
		// The new row data.
		if (m_width < w)
			row[i] = m_row[i] << (w - m_width);
		else if (w < m_width)
			row[i] = m_row[i] >> (m_width - w);
		else
			row[i] = m_row[i];
		// The new inset data.
		if (m_row[i] == 0)
			inset[i] = w;
		else
			inset[i] = m_inset[i];
	}
	if (m_height != h) {
		// If we're adding rows, make sure they are blanked.
		for (int i = m_height; i < h; ++i) {
			row[i] = 0;
			inset[i] = w;
		}
		delete[] m_row;
		delete[] m_inset;
		m_row = row;
		m_inset = inset;
	}
	m_height = h;
	m_width = w;
	assert(isValid());
}

bool MazezamData::checkInsets(const mzm_coord* inset) const
{
	for (int i = 0; i < m_height; i++) {
		assert(inset[i] >= 0);
		if (m_row[i]) {
			// Check the row is left aligned.
			assert(m_row[i] & (1 << (m_width - 1)));
			// Check the row is zero beyond the first block.
			assert((m_row[i] & ~((mzm_row)((1 << m_width) - 1))) == 0);
			if (inset[i]) 
			{
				// Check that the row can fit when adjusted by the inset.
				assert(0 == (m_row[i] & (1 << (inset[i] - 1)))); 
			}
		} else
			assert (inset[i] == m_width);
	}
	return true;
}

bool MazezamData::isValid() const
{
	assert (m_height >= MIN_MAZEZAM_HEIGHT);
	assert (m_height <= MAX_MAZEZAM_HEIGHT);
	assert (m_width >= MIN_MAZEZAM_WIDTH);
	assert (m_width <= MAX_MAZEZAM_WIDTH);
	// The row data fits in the size.
	for (int i = 0; i < m_height; ++i) {
		if (m_width < MAX_MAZEZAM_WIDTH)
			assert (m_row[i] < (mzm_row) (1 << m_width));
	}
	assert((m_start >= 0) && (m_start < m_height));
	assert((m_finish >= 0) && (m_finish < m_height));
	assert (checkInsets(m_inset));
	return true;
}

bool MazezamData::checkStartPosition() const
{
	assert (m_inset[(unsigned char) m_start] > 0);
	return true;
}


void MazezamData::shiftWest(mzm_coord r)
{
	assert(canShiftWest(r));
	if (m_row[r])
		--m_inset[r];
}

void MazezamData::shiftEast(mzm_coord r)
{
	assert(canShiftEast(r));
	if (m_row[r])
		++m_inset[r];
}

void MazezamData::setBlock(mzm_coord y, mzm_coord x, bool b) {
	assert((y >= 0) && (y < m_height));
	assert((x >= 0) && (x < m_width));
	// Check we're not putting a block in front of the start position.
	//assert( (!b) || (y != m_start) || (x != 0));
	if (getBlock(y,x) == b)
		return;
	if (b) {
		if (x < m_inset[y]) {
			m_row[y] >>= (m_inset[y] - x);
			m_inset[y] = x;
			m_row[y] |= 1 << (m_width - 1);
		} else {
			m_row[y] |= 1 << (m_width + m_inset[y] - x - 1);
		}
	} else {
		if (x > m_inset[y]) {
			m_row[y] &= ~(1 << (m_width + m_inset[y] - x - 1));	
		} else {
			m_row[y] &= ~(1 << (m_width - 1));
			if (m_row[y]) {
				do {
					m_row[y] <<= 1;
					++m_inset[y];
				} while (!(m_row[y] & (1 << (m_width - 1))));
			} else
				m_inset[y] = m_width;
		}
	}
	assert(isValid());
}

void MazezamData::setStart(mzm_coord y) {
	assert((y >= 0) && (y < m_height));
	assert((m_inset[y] > 0) || (m_row[y] == 0));
	m_start = y;
}

void MazezamData::setFinish(mzm_coord y) {
	assert((y >= 0) && (y < m_height));
	m_finish = y;
}

void MazezamData::clear()
{
	for (mzm_coord i = 0; i < m_height; ++i) {
		m_row[i] = 0;
		m_inset[i] = m_width;
	}
}

std::ostream& operator<< (std::ostream& os, const MazezamData& l) 
{
	for (int j = 0; j < l.getWidth() + 2; ++j)
		os << CHAR_WALL;
	os << std::endl;
	for (int i = 0; i < l.getHeight(); ++i) {
		os << ( (i == l.getStart()) ? CHAR_ENTRANCE : CHAR_WALL );
		for (int j = 0; j < l.getWidth(); ++j)
			os << ( l.getBlock(i,j) ? CHAR_BLOCK : CHAR_SPACE );
		os << ( (i == l.getFinish()) ? CHAR_EXIT : CHAR_WALL ) << std::endl;
	}
	for (int j = 0; j < l.getWidth() + 2; ++j)
		os << CHAR_WALL;
	os << std::endl;
	return os;	
}

} // namespace mzm