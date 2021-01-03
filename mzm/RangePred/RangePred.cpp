/* ***************************************************************************
 * RangePred.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <cctype>

#include "RangePred.hpp"

const int POS_INFINITY = -1;
const int NEG_INFINITY = 0;

bool LessWithInfinities::operator() (int n, int m) const
{
	return (n == NEG_INFINITY) || (m == POS_INFINITY) || ((n != POS_INFINITY) && (n < m));
}

bool BelowWithInfinities::operator() (int n, int m) const
{
	return (n == NEG_INFINITY) || (m == POS_INFINITY) || ((n != POS_INFINITY) && (n < m - 1));
}

RangePred::RangePred() {}

RangePred::~RangePred() {}

bool RangePred::operator() (int n) const
{
	return contains(n);
}

/*!
 * Maps characters to array indices used by the transition function in addArgument.
 *   COMMA -> 0
 *   DIGIT -> 1
 *   MINUS -> 2
 *   NULL  -> 3
 *   ...   -> 4
 */
int g(char c)
{
	if (c == ',')
		return 0;
	else if (isdigit(c))
		return 1;
	else if (c == '-')
		return 2;
	else
		return 3;
}

void RangePred::newArgument(const char* arg)
{
	int num0 = 0;
	int num1 = 0;
	
	// The parser has the following states.
	enum states {
		NEW_PHRASE,   // We've just started parsing a new phrase. 
		NUM,          // We're parsing an individual, a rightInterval or the first number in an interval.
		PRE_L,        // We've just parsed a dash for a left interval.
		NUM_L,        // We're parsing the number in a left interval.
		R_OR_I,       // We've parsed a number and then a dash.
		NUM_I,        // We're parsing the second number in an interval.
		FINISH_1,     // We've finished parsing an individual, e.g. "5".
		FINISH_L,     // We've just parsed a leftInterval, e.g. "-3".
		FINISH_R,     // We've just parsed a rightInterval, e.g. "10-".
		FINISH_I,     // We've just parsed an interval, e.g. "6-8".
		ERROR,
		NUM_STATES   // The number of states.
	} state = NEW_PHRASE;
	
	// state transitions, the second axis is f(c).
	states f_state[NUM_STATES][5] = {
		// COMMA, DIGIT, DASH, END, OTHER
		{ERROR,    NUM,	  PRE_L,  ERROR,    ERROR},
		{FINISH_1, NUM,	  R_OR_I, FINISH_1, ERROR},
		{ERROR,    NUM_L, ERROR,  ERROR,    ERROR},
		{FINISH_L, NUM_L, ERROR,  FINISH_L, ERROR},
		{FINISH_R, NUM_I, ERROR,  FINISH_R, ERROR},
		{FINISH_I, NUM_I, ERROR,  FINISH_I, ERROR},
		{ERROR,    NUM,   PRE_L,  ERROR,    ERROR},
		{ERROR,    NUM,   PRE_L,  ERROR,    ERROR},
		{ERROR,    NUM,   PRE_L,  ERROR,    ERROR},
		{ERROR,    NUM,   PRE_L,  ERROR,    ERROR},
		{ERROR,    ERROR, ERROR,  ERROR,    ERROR}
	};
	
	int i = 0;
	char c;
	do {
		c = arg[i++];
		state = f_state[state][g(c)];
		switch(state) {
			case ERROR:
				throw BadArgument();
				break;
			case NUM:
			case NUM_L:
				num0 = (num0 * 10) + (c - '0');
				break;
			case NUM_I:
				num1 = (num1 * 10) + (c - '0');
				break;
			case FINISH_1:
				if (num0 == 0)
					throw BadArgument();
				insert(num0,num0);
				num0 = 0;
				break;
			case FINISH_L:
				if (num0 == 0)
					throw BadArgument();
				insert(NEG_INFINITY,num0);
				num0 = 0;
				break;
			case FINISH_R:
				if (num0 == 0)
					throw BadArgument();
				insert(num0,POS_INFINITY);
				num0 = 0;
				break;
			case FINISH_I:
				if ((num0 == 0) || (num1 == 0) || (num1 < num0))
					throw BadArgument();
				insert(num0,num1);
				num0 = 0;
				num1 = 0;
				break;
			default:
				break;
		}
	} while (c != '\0');
}
