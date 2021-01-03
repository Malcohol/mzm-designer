/* ***************************************************************************
 * SolverTypes.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef SOLVERTYPES_H_
#define SOLVERTYPES_H_

namespace mzmslv {

/*!
 * The types of search used by the solver class.
 */
enum SearchType {
	BREADTH_FIRST,
	DEPTH_FIRST,
	BEST_FIRST,
	A_STAR
};

/*!
 * The outcomes of running a solver. 
 */
enum SolverResult {
	/*!
	 * The solvers return this if they find a solution.
	 */
	FOUND_SOLUTION,
	/*!
	 * The solvers return this if there is no solution.
	 */
	NO_SOLUTION,
	/*!
	 * The solvers return this if they were interrupted.
	 */
	INTERRUPTED
};

} // namespace mzmslv

#endif /*SOLVERTYPES_H_*/
