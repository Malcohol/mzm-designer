/* ***************************************************************************
 * TypesAndConstants.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAM_TYPES_H_
#define MAZEZAM_TYPES_H_

#include<vector>
#include<ostream>

namespace mzm {

/*!
 * Type for a coord or a dimension in a mazezam level.
 */
typedef signed char mzm_coord;
/*!
 * Type for a row of level data encoded in binary.
 * We use binary encodings to allow an efficient encoding of the zone in ConfigEqv.
 */
typedef unsigned int mzm_row;

/*!
 * The maximum height of a mazezam supported by the program.
 */
extern const int MAX_MAZEZAM_HEIGHT;
/*!
 * The minimum height of a mazezam supported by the program.
 */
extern const int MIN_MAZEZAM_HEIGHT;
/*!
 * The maximum width of a mazezam supported by the program.
 * This is defined to be the same size as the number of bits in an mzm_row.
 */
extern const int MAX_MAZEZAM_WIDTH;
/*!
 * The minimum width of a mazezam supported by the program.
 */
extern const int MIN_MAZEZAM_WIDTH;

/*!
 * The directions in which the player can move.
 */
enum mzm_direction {
	DIR_NORTH,
	DIR_SOUTH,
	DIR_EAST,
	DIR_WEST,
	DIR_PUSH_EAST,
	DIR_PUSH_WEST
};

/*!
 * Stores a sequence of mzm_directions.
 * This never stores the initial or final EAST moves.
 */
typedef std::vector<mzm_direction> mzm_path;

/*!
 * The characters used to input and output Mazezams.
 */
static const char CHAR_SPACE = ' ';
static const char CHAR_PERSON = '@';
static const char CHAR_BLOCK = '$';
static const char CHAR_WALL = '#';
static const char CHAR_ENTRANCE = '+';
static const char CHAR_EXIT = '*';
static const char CHAR_ZONE = ',';

/*!
 * The characters used to input and output directions.
 */
static const char directionChars[] = {'u', 'd', 'r', 'l', 'R', 'L'};

/*!
 * Write a direction to an ostream.
 */
std::ostream& operator<< (std::ostream& os, const mzm_direction& d);

/*!
 * Write a path to an ostream. 
 */
std::ostream& operator<< (std::ostream& os, const mzm_path& p);

} // namespace mzm

#endif /*MAZEZAM_TYPES_H_*/
