/* ***************************************************************************
 * MazezamSolutionType.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "MazezamSolutionType.hpp"

namespace mzm {

MazezamSolutionType extractNextSolutionType(unsigned int& flags)
{
	int f = 1;
	const int flagTooFar = 1 << MAZEZAM_SOLUTION_NUM_TYPES; 
	while ( (f < flagTooFar) && !(flags & f) )
		f = f << 1;
	flags &= ~f;
	return MazezamSolutionType(f);
}

unsigned int getSearches(unsigned int& flags)
{
	unsigned int searches = flags & MAZEZAM_SOLUTION_SEARCHES;
	if ((flags & MAZEZAM_SOLUTION_RATING) || (flags & MAZEZAM_SOLUTION_NUM_PUSHES))
		searches |= MAZEZAM_SOLUTION_FEWEST_PUSHES;
	if (flags & MAZEZAM_SOLUTION_NUM_MOVES)
		searches |= MAZEZAM_SOLUTION_FEWEST_MOVES;
	return searches; 
}

} // namespace mzm
