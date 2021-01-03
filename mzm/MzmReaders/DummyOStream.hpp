/* ***************************************************************************
 * DummyOStream.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef DUMMYOSTREAM_H_
#define DUMMYOSTREAM_H_

/*!
 * An ostream which does nothing when characters are written to it.
 * This has only been tested with operator<<, so I can't guarantee how
 * robust it is as a general ostream class. 
 */
class DummyOStream : public std::ostream {
	/*!
	 * The hack is to modify a std::streambuf to ignore overflows.
	 */
	class DummyOBuf : public std::streambuf {
		int_type overflow (int_type c) { return c; }
	};
	/*!
	 * Have the stream own its buffer.
	 */
	DummyOBuf d;
public:
	/*!
	 * Constructor.
	 */
	DummyOStream() : std::ostream(&d) { } 
};

#endif /*DUMMYOSTREAM_H_*/
