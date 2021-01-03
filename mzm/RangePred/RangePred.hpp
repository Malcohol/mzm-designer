/* ***************************************************************************
 * RangePred.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef RANGELIST_H_
#define RANGELIST_H_

#include "IntervalSet.hpp"

/*!
 * A comparison class on non-negative integers with a positive and negative
 * infinity value.  
 */
class LessWithInfinities
{
public:
	/*!
	 * Test if n is less than m when n and m may denote infinity.
	 * \param n a positive int or one of the infinities.
	 * \param m a positive int or one of the infinities.
	 * \return true if n less than m.
	 */
	bool operator() (int n, int m) const;
};

/*!
 * A comparison class on non-negative integers with a positive and negative
 * infinity value, which gives false if n = m - 1.
 */
class BelowWithInfinities
{
public:
	/*!
	 * Test if n is below m (that is, less than and not next to) when n and m may denote infinity.
	 * \param n a positive int or one of the infinities.
	 * \param m a positive int or one of the infinities.
	 * \return true if n less than and not next to m.
	 */
	bool operator() (int n, int m) const;
};

/*!
 * A class which builds up a predicate on positive integers using range
 * expressions such as "4,-2,5,6-10,14,15,19-".
 * This does the parsing and uses IntervalSet to manage the storage.
 */
class RangePred : private IntervalSet<int,LessWithInfinities,BelowWithInfinities>
{
public:
	/*!
	 * Thrown by the constructor or newArgument if the argument is not a valid range specifier.
	 */
	class BadArgument {};

	/*!
	 * Construct a RangeList. 
	 */
	RangePred();
	
	/*!
	 * Destructor.
	 */
	virtual ~RangePred();
	
	/*!
	 * Augment the range with the provided range specifier.
	 * \param arg a range specifier to add to the range.
	 * \throw BadArgument if the arg cannot be correctly parsed.
	 */
	void newArgument(const char* arg);
	
	/*!
	 * The predicate operation.
	 * \param n the integer to test.
	 * \return true iff n is in the range.
	 */
	bool operator() (int n) const;
};

#endif /*RANGELIST_H_*/
