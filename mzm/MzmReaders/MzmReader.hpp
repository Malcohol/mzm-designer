/* ***************************************************************************
 * MzmReader.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MZMREADER_H_
#define MZMREADER_H_

#include <mzm/Mazezam/MazezamData.hpp>

namespace mzm {

/*!
 * An interface for classes used to read mazezam levels, one level at a time.
 */
class MzmReader
{
public:
	/*!
	 * Thrown by getLevel if there is currently no level.
	 */
	class NoLevel {};
	/*!
	 * Thrown by next if there was a parsing error.
	 */
	class ParseError {
		public:
			ParseError (int line) : m_line(line) {};
			int getLine() const { return m_line; };
			virtual void writeToStream (std::ostream& os) { os << "Parse error on line " << m_line; } 
		protected:	
			const int m_line;
	};
	
	/*!
	 * Returns a reference to last level parsed.
	 */
	virtual MazezamData* getLevel() const = 0;
	
	/*!
	 * Returns the level number of the last level parsed.
	 */
	virtual int getLevelNumber() const = 0;
	
	/*!
	 * Attempt to parse the next level, return true if it succeeds.
	 */
	virtual bool next() = 0;
	
	/*!
	 * Copy the current level to the output stream, if one was provided.
	 */
	virtual void copyLevel() = 0;
	
	/*!
	 * Destructor.
	 */
	virtual ~MzmReader() {}
};

} // namespace mzm

#endif /*MZMREADER_H_*/
