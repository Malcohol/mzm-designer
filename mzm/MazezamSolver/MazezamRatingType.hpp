/* ***************************************************************************
 * MazezamRatingType.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMRATINGTYPE_H_
#define MAZEZAMRATINGTYPE_H_

namespace mzm {

typedef float MazezamRating;

#define STRINGIFY(s) #s
#define MAZEZAM_RATING_ESCAPE_STRING(numchars) "%" STRINGIFY(numchars) ".2f"

#define MAZEZAM_RATING_UNSOLVABLE -1.0f

} // namespace mzm

#endif /*MAZEZAMRATINGTYPE_H_*/
