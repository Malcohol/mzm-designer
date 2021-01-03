/* ***************************************************************************
 * MazezamSolverJob.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "MazezamSolverJob.hpp"

namespace mzm {

mzmslv::WorkerPoolJob* createMazezamSolverJob(const MazezamData& m, MazezamSolutionType type)
{
	switch(type) {
		case MAZEZAM_SOLUTION_FEWEST_PUSHES:
			return new MazezamSolverJobFewestPushes(m);
		case MAZEZAM_SOLUTION_FEWEST_MOVES:
			return new MazezamSolverJobFewestMoves(m);
		case MAZEZAM_SOLUTION_FASTEST:
			return new MazezamSolverJobFastest(m);
		default:
			assert(false);
	}
}

} // namespace mzm