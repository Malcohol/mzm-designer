/* ***************************************************************************
 * MazezamSolutionSource.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMSOLUTIONSOURCE_H_
#define MAZEZAMSOLUTIONSOURCE_H_

namespace mzm {

/*!
 * A MazezamSolutionSource can be queried for solutions.
 */
class MazezamSolutionSource
{
public:
	/*!
	 * Is there a solution?
	 */
	virtual bool isSolvable() const = 0;
	/*!
	 * Add the solution to the path.
	 */
	virtual void addToMzmPath(mzm_path& path) const = 0;
};

} // namespace mzm

#endif /*MAZEZAMSOLUTIONSOURCE_H_*/
