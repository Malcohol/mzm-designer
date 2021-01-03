/* ***************************************************************************
 * MazezamData.inl
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/
namespace mzm {

bool MazezamData::getBlock(mzm_coord y, mzm_coord x) const {
	assert(y < m_height);
	assert(x < m_width);
	return ((m_row[y] >> m_inset[y]) & (1 << ((m_width - 1) - x)));
}

bool MazezamData::canShiftWest(mzm_coord r) const
{
	return (m_inset[r] > 0);
}

bool MazezamData::canShiftEast(mzm_coord r) const
{
	return (((m_row[r] >> m_inset[r]) & 1) == 0);
}

mzm_row MazezamData::getRow(mzm_coord r) const
{
	assert (r < m_height);
	return m_row[r];
}

mzm_coord MazezamData::getInset(mzm_coord r) const
{
	assert (r < m_height);
	return m_inset[r];
}

mzm_coord MazezamData::getHeight() const {
	return m_height;
}

mzm_coord MazezamData::getWidth() const {
	return m_width;
}

mzm_coord MazezamData::getStart() const {
	return m_start;
}

mzm_coord MazezamData::getFinish() const {
	return m_finish;
}

} // namespace mzm
