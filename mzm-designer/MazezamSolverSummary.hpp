/* ***************************************************************************
 * MazezamSolverSummary.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMSOLVERSUMMARY_H_
#define MAZEZAMSOLVERSUMMARY_H_

#include <mzm/Mazezam/TypesAndConstants.hpp>
#include <mzm/MazezamSolver/MazezamRatingType.hpp>

/*!
 * Collects useful information about the solver outcome.
 */
struct MazezamSolverSummary {
	/*!
	 * Constructor.
	 */
	MazezamSolverSummary() : m_validSolutionFlags(0) {}
	/*!
	 * Which of the rating / pushes / moves are valid.
	 */
	unsigned int m_validSolutionFlags;
	/*!
	 * The result of the solver process.
	 * The other three members are non-zero/non-empty iff this equals FOUND_SOLUTION.
	 */
	bool m_isSolvable;
	/*!
	 * A rating of the mazezam.
	 */
	mzm::MazezamRating m_rating;
	/*!
	 * The solution found (as an mzm_path).
	 */
	mzm::mzm_path m_solution;
	/*!
	 * The number of pushes in the solution.
	 */
	unsigned int m_numPushes;
	/*!
	 * The number of moves in the solution.
	 */
	unsigned int m_numMoves;
};

#endif /*MAZEZAMSOLVERSUMMARY_H_*/
