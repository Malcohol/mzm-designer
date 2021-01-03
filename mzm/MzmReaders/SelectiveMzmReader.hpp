/* ***************************************************************************
 * SelectiveMzmReader.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef SELECTIVEMZMREADER_H_
#define SELECTIVEMZMREADER_H_

#include <mzm/MzmReaders/MzmReader.hpp>
#include <mzm/RangePred/RangePred.hpp>

namespace mzm {

/*!
 * An MzmReader which wraps another MzmReader with a RangePredicate, so that
 * only the selected levels are returned.
 */
class SelectiveMzmReader : public MzmReader
{
public:
	/*!
	 * The MzmReader and RangePred are expected to live for the duration
	 * of the object. 
	 */
	SelectiveMzmReader(MzmReader& mzmReader, const RangePred& rangePred);
	
	// The MzmReader interface.
	virtual MazezamData* getLevel() const;
	virtual int getLevelNumber() const;
	virtual bool next();
	virtual void copyLevel();	
private:
	/*!
	 * The MzmReader from which levels are read.
	 */
	MzmReader& m_mzmReader;
	/*!
	 * The RangePred which selects the appropriate levels.
	 */
	const RangePred& m_rangePred;
};

} // namespace mzm

#endif /*SELECTIVEMZMREADER_H_*/
