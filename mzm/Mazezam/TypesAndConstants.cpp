/* ***************************************************************************
 * TypesAndConstants.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <mzm/Mazezam/TypesAndConstants.hpp>

#include<climits>

namespace mzm {

const int MAX_MAZEZAM_HEIGHT = 255;
const int MIN_MAZEZAM_HEIGHT = 2;
// The number of bits in an mzm_row is the number of chars times the number
// bits in a char.
// TODO - work out why I need the "- 1"
// Currently the solver thinks maximal width mazezams are unsolvable.
const int MAX_MAZEZAM_WIDTH = sizeof(mzm_row) * CHAR_BIT - 1;
const int MIN_MAZEZAM_WIDTH = 4;

std::ostream& operator<< (std::ostream& os, const mzm_direction& d)
{
	os << directionChars[d];
	return os;
}

std::ostream& operator<< (std::ostream& os, const mzm_path& p)
{
	for (mzm_path::const_iterator i = p.begin(); i != p.end(); ++i)
		os << *i;
	return os;
}

} // namespace mzm