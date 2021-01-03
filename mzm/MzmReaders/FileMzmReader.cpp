/* ***************************************************************************
 * FileMzmReader.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <cassert>
#include <cctype>

#include <mzm/MzmReaders/FileMzmReader.hpp>

namespace mzm {

FileMzmReader::FileMzmReader(std::istream& is, const std::string& fileName, std::ostream& os) :
	m_istream(is), m_ostream(os), m_fileName(fileName), m_lineNumber(1), m_hasLevel(false), m_number(0)
{
	assert(is);
	assert(os);
}

MazezamData* FileMzmReader::getLevel() const
{
	if (!m_hasLevel)
		throw NoLevel();
	return new MazezamData(*m_level);
}

int FileMzmReader::getLevelNumber() const
{
	if (!m_hasLevel)
		throw NoLevel();
	return m_number;
}

FileMzmReader::~FileMzmReader()
{
	if (m_hasLevel) {
		delete m_level;
	}
}

/*!
 * A mapping from characters to indices which is used in findNextMazezam and parseRestOfMazezam. 
 */
int f(char c) {
	// \n, WALL, BLOCK, START, FINISH, SPACE, other
	switch (c) {
		case '\n':
			return 0;
		case CHAR_WALL:
			return 1;
		case CHAR_BLOCK:
			return 2;
		case CHAR_ENTRANCE:
			return 3;
		case CHAR_EXIT:
			return 4;
		case CHAR_SPACE:
			return 5;
		default:
			return 6;
	}
}

bool FileMzmReader::findNextMazezam(mzm_coord& width)
{
	char c;
	
	// The parser has the following states.
	enum states {
		SEEKING,      // We're still looking for the start of a solid wall.
		FOUND_WALL,   // We've finished parsing a line with a solid wall.
		NEW_LINE,     // We've just started a new line.
		SOLID_WALL,   // We're parsing a solid wall.
		AFTER_WALL,   // We've finished parsing a solid wall.
		ERROR,        // We're in an error state.
		NUM_STATES    // The number of states.
	} state = NEW_LINE;
	
	// state transitions, the second axis is f(c).
	states f_state[NUM_STATES][7] = {
		// NEWLINE, WALL, BLOCK, START, FINISH, SPACE, OTHER
		{NEW_LINE, SEEKING, SEEKING, SEEKING, SEEKING, SEEKING, SEEKING},
		{ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR},
		{NEW_LINE, SOLID_WALL, SEEKING, SEEKING, SEEKING, SEEKING, SEEKING},
		{FOUND_WALL, SOLID_WALL, ERROR, ERROR, ERROR, AFTER_WALL, ERROR},
		{FOUND_WALL, ERROR, ERROR, ERROR, ERROR, AFTER_WALL, ERROR},
		{ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}
	};
	
	width = 0;
	
	// seek the north wall of a mazezam.
	while (state != FOUND_WALL && m_istream.get(c)) {
		state = f_state[state][f(c)];
		switch (state) {
			case ERROR:
				errorIf(true);
				break;
			case SEEKING:
				m_ostream << c;
				break;
			case SOLID_WALL:
				errorIf(width >= MAX_MAZEZAM_WIDTH);
				++width;
				break;
			case NEW_LINE:
				++m_lineNumber;
				m_ostream << std::endl;
				break;
			default:
				break;
		}
	}
	// Width is 2 too big.
	width-=2;
	++m_lineNumber;
	return state == FOUND_WALL; 
}

void FileMzmReader::parseRestOfMazezam(MazezamData::array_type& block, mzm_coord width, mzm_coord& height, mzm_coord& start, mzm_coord& finish)
{
	char c;
	height = 0;
	start = 0;
	finish = 0;
	
	// The parser has the following states.
	enum states { 
		FOUND_WALL,      // We've finished parsing a line with a solid wall. 
		FOUND_LINE,      // We've finished parsing a line with a mazezam line. 
		NEW_LINE,        // We've just started parsing a line.
		SOLID_WALL,      // We're parsing a solid wall.
		AFTER_WALL,      // We've finished parsing a solid wall.
		LINE_PRE_BLOCK,  // We're parsing a line, but have not yet encountered a block.
		LINE_POST_BLOCK, // We're parsing a line, and have encountered a block.
		AFTER_LINE,      // We've finished parsing a mazezam line.
		WALL_OR_LINE,    // We've read the first character of either a wall or a line.
		ERROR,           // We're read a character we shouldn't have.
		NUM_STATES       // The number of states.
	} state = NEW_LINE;
	
	// state transitions, the second axis is f(c).
	states f_state[NUM_STATES][7] = {
		// NEWLINE, WALL, BLOCK, START, FINISH, SPACE, OTHER
		{ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR},
		{ERROR, WALL_OR_LINE, ERROR, WALL_OR_LINE, ERROR, ERROR, ERROR},
		{ERROR, WALL_OR_LINE, ERROR, WALL_OR_LINE, ERROR, ERROR, ERROR},
		{FOUND_WALL, SOLID_WALL, ERROR, ERROR, ERROR, AFTER_WALL, ERROR},
		{FOUND_WALL, ERROR, ERROR, ERROR, ERROR, AFTER_WALL, ERROR},
		{ERROR, AFTER_LINE, LINE_POST_BLOCK, ERROR, AFTER_LINE, LINE_PRE_BLOCK, ERROR},
		{ERROR, AFTER_LINE, LINE_POST_BLOCK, ERROR, AFTER_LINE, LINE_POST_BLOCK, ERROR},
		{FOUND_LINE, ERROR, ERROR, ERROR, ERROR, AFTER_LINE, ERROR},
		{ERROR, SOLID_WALL, LINE_POST_BLOCK, ERROR, ERROR, LINE_PRE_BLOCK, ERROR},
		{ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}
	};
	
	// a local width value.
	int w = 0;
	bool foundStart = false;
	bool foundFinish = false;
		
	// keep processing mazezam lines until we find the south wall.
	while (state != FOUND_WALL && m_istream.get(c)) {
		state = f_state[state][f(c)];
		switch (state) {
			case ERROR:
				errorIf(true);
				break;
			case LINE_PRE_BLOCK:
			case LINE_POST_BLOCK:
				++w;
				block[height].push_back(c == CHAR_BLOCK);
				break;
			case SOLID_WALL:
				++w;
				break;
			case FOUND_LINE:
				errorIf (w != width);
				w = 0;
				errorIf (height == MAX_MAZEZAM_HEIGHT);
				++height;
				++m_lineNumber;
				break;
			case FOUND_WALL:
				// walls always count one more for width.
				errorIf (w != width + 1);
				++m_lineNumber;
				break;
			default:
				break;
		}
		if (c == CHAR_ENTRANCE) {
			errorIf (foundStart);
			foundStart = true;
			start = height;
		} else if (c == CHAR_EXIT) {
			errorIf(foundFinish);
			foundFinish = true;
			finish = height;
		}
	}
	errorIf (state != FOUND_WALL);
	errorIf (height < 1);
	errorIf (!(foundStart && foundFinish));
}

bool FileMzmReader::next()
{
	// delete any storage due to the previous level if there was one.
	if (m_hasLevel) {
		delete m_level;
		m_hasLevel = false;	
	}
	
	// temporary structures into which the level will be parsed.
	MazezamData::array_type new_array(MAX_MAZEZAM_HEIGHT);
	mzm_coord width = 0, height = 0, start = 0, finish = 0;
	
	// look for a solid north wall.
	if (!findNextMazezam(width))
		return false;
	++m_number;
	
	// parse the rest of the mazezam.
	parseRestOfMazezam(new_array,width,height,start,finish);
	
	// construct the mzm_t object.
	m_level = new MazezamData(new_array, height, width, start, finish);	
	
	m_hasLevel = true;
	return true;
}

void FileMzmReader::copyLevel()
{
	if (!m_hasLevel)
		throw NoLevel();
	m_ostream << *m_level;
}

std::ostream& operator<<(std::ostream& os, FileMzmReader::FileParseError e)
{
	e.writeToStream(os);
	return os;
}

} // namespace mzm