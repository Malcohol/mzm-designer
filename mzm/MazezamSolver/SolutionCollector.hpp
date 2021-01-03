/* ***************************************************************************
 * SolutionCollector.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef SOLUTIONCOLLECTOR_H_
#define SOLUTIONCOLLECTOR_H_

#include <mzm/Mazezam/TypesAndConstants.hpp>
#include <mzm/MazezamSolver/MazezamSolutionType.hpp>
#include <mzm/MazezamSolver/MazezamRatingType.hpp>
#include <mzm/MazezamSolver/MazezamSolutionSource.hpp>

namespace mzm {

class MazezamData;

/*!
 * An interface for classes that receive callbacks about mazezam solutions.
 */
class SolutionCollector
{
public:
	/*!
	 * Inform the collector that this level comes next in the sequence.
	 */
	virtual void collectLevelNumber(unsigned int levelNumber) {}
	/*!
	 * Inform the collector about a solution to the mazezam.
	 */
	virtual void collectSolution(unsigned int levelNumber, MazezamSolutionType solutionType, const MazezamSolutionSource& source) = 0;
	/*!
	 * Inform the collector that a level has been assigned the rating.
	 */
	virtual void collectRating(unsigned int levelNumber, MazezamRating rating) {}
	/*!
	 * Inform the collector of the minimum number of pushes required to solve the level.
	 */
	virtual void collectNumPushes(unsigned int levelNumber, unsigned int numPushes) {}
	/*!
	 * Inform the collector of the minimum number of moves required to solve the level.
	 */
	virtual void collectNumMoves(unsigned int levelNumber, unsigned int numMoves) {}
	/*!
	 * Informs the collector that an improved version of the level has been obtained.
	 */
	virtual void collectImprovement(unsigned int levelNumber, const MazezamData& level) {}
	/*!
	 * Virtual destructor.
	 */
	virtual ~SolutionCollector() {}
};

} // namespace mzm

#endif /*SOLUTIONCOLLECTOR_H_*/
