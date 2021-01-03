/* ***************************************************************************
 * MazezamSolutionType.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMSOLUTIONTYPE_H_
#define MAZEZAMSOLUTIONTYPE_H_

namespace mzm {

/*!
 * Used to count the types of solution and set the appropriate bit in the flags of MazezamSolutionType.
 */
enum MazezamSolutionTypeIndices {
	MAZEZAM_SOLUTION_FEWEST_PUSHES_INDEX,
	MAZEZAM_SOLUTION_FEWEST_MOVES_INDEX,
	MAZEZAM_SOLUTION_FASTEST_INDEX,
	
	MAZEZAM_SOLUTION_MAX_SEARCHES_INDEX = MAZEZAM_SOLUTION_FASTEST_INDEX,
	
	MAZEZAM_SOLUTION_RATING_INDEX,
	MAZEZAM_SOLUTION_NUM_PUSHES_INDEX,
	MAZEZAM_SOLUTION_NUM_MOVES_INDEX,
	/*!
	 * The number of solution types.
	 */
	MAZEZAM_SOLUTION_NUM_TYPES,
};

/*!
 * The differed types of solutions available.
 */
enum MazezamSolutionType {
	MAZEZAM_SOLUTION_INVALID_TYPE  = 0,
	
	MAZEZAM_SOLUTION_FEWEST_PUSHES = 1 << MAZEZAM_SOLUTION_FEWEST_PUSHES_INDEX,
	MAZEZAM_SOLUTION_FEWEST_MOVES  = 1 << MAZEZAM_SOLUTION_FEWEST_MOVES_INDEX,
	MAZEZAM_SOLUTION_FASTEST       = 1 << MAZEZAM_SOLUTION_FASTEST_INDEX,
	
	MAZEZAM_SOLUTION_SEARCHES      = (1 << (MAZEZAM_SOLUTION_MAX_SEARCHES_INDEX + 1)) - 1,

	MAZEZAM_SOLUTION_RATING        = 1 << MAZEZAM_SOLUTION_RATING_INDEX,
	MAZEZAM_SOLUTION_NUM_PUSHES    = 1 << MAZEZAM_SOLUTION_NUM_PUSHES_INDEX,
	MAZEZAM_SOLUTION_NUM_MOVES     = 1 << MAZEZAM_SOLUTION_NUM_MOVES_INDEX, 
	
	MAZEZAM_SOLUTION_ALL           = (1 << MAZEZAM_SOLUTION_NUM_TYPES) - 1
};

/*!
 * Given a bitfield representing a selection of solution types, 
 * extract one and return it. 
 */
MazezamSolutionType extractNextSolutionType(unsigned int& flags);

/*!
 * Return the searches corresponding to the solution types specified.
 */
unsigned int getSearches(unsigned int& flags);

} // namespace mzm

#endif /*MAZEZAMSOLUTIONTYPE_H_*/
