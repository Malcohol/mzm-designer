/* ***************************************************************************
 * SelectiveMzmReader.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <mzm/MzmReaders/SelectiveMzmReader.hpp>

namespace mzm {

SelectiveMzmReader::SelectiveMzmReader(MzmReader& mzmReader, const RangePred& rangePred)
: m_mzmReader(mzmReader)
, m_rangePred(rangePred)
{
}

MazezamData* SelectiveMzmReader::getLevel() const
{
	return m_mzmReader.getLevel();
}

int SelectiveMzmReader::getLevelNumber() const
{
	return m_mzmReader.getLevelNumber();
}

bool SelectiveMzmReader::next()
{
	bool isNextLevel;
	while ( (isNextLevel = m_mzmReader.next()) && !m_rangePred(m_mzmReader.getLevelNumber()) )
		m_mzmReader.copyLevel();
	return isNextLevel;
}

void SelectiveMzmReader::copyLevel()
{
	m_mzmReader.copyLevel();
}

} // namespace mzm
