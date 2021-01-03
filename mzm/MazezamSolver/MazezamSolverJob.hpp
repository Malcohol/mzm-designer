/* ***************************************************************************
 * MazezamSolverJob.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMSOLVERJOB_H_
#define MAZEZAMSOLVERJOB_H_

#include <cassert>
#include <cmath>

#include <mzm/MazezamSolver/Config.hpp>
#include <mzm/MazezamSolver/ConfigEqv.hpp>
#include <mzmslv/SolverJob.hpp>
#include <mzm/MazezamSolver/MazezamSolutionType.hpp>
#include <mzm/MazezamSolver/MazezamSolutionSource.hpp>
#include <mzm/MazezamSolver/MazezamRatingType.hpp>

namespace mzm {

/*!
 * A worker job which solves Mazezams.
 * Subclasses for specific use-cases are provided below.
 */
template<class C>
class MazezamSolverJob : protected MazezamData, public mzmslv::SolverJob<C>, public MazezamSolutionSource
{
public:
	/*!
	 * Get an mzmPath corresponding to the solution.
	 */
	virtual void addToMzmPath(mzm_path& path) const;
	/*!
	 * Return if the mazezam is solvable.
	 */
	virtual bool isSolvable() const;
	/*!
	 * Get a rating of the mazezam using this solution.
	 */
	MazezamRating getRating() const;
	/*!
	 * Get a const reference to the level which was solved.
	 */
	const MazezamData& getLevel() const;
	/*!
	 * Get the type of solution.
	 */
	virtual MazezamSolutionType getType() const { return MAZEZAM_SOLUTION_INVALID_TYPE; }
protected:
	/*!
	 * Constructor.
	 */
	MazezamSolverJob(const MazezamData& m, mzmslv::SearchType type);
};

/*!
 * Factory method.
 */
mzmslv::WorkerPoolJob* createMazezamSolverJob(const MazezamData& m, MazezamSolutionType type);

/*!
 * A WorkerPoolJob for solving mazezams which finds a solution with the fewest moves.
 */
class MazezamSolverJobFewestMoves : public MazezamSolverJob<Config>
{
public:
	MazezamSolverJobFewestMoves(const MazezamData& m) : MazezamSolverJob<Config>(m, mzmslv::BREADTH_FIRST) {}
	virtual MazezamSolutionType getType() const { return MAZEZAM_SOLUTION_FEWEST_MOVES; }	
};

/*!
 * A WorkerPoolJob for solving mazezams which finds a solution with the fewest pushes.
 */
class MazezamSolverJobFewestPushes : public MazezamSolverJob<ConfigEqv>
{
public:
	MazezamSolverJobFewestPushes(const MazezamData& m) : MazezamSolverJob<ConfigEqv>(m, mzmslv::BREADTH_FIRST) {}
	virtual MazezamSolutionType getType() const { return MAZEZAM_SOLUTION_FEWEST_PUSHES; }	
};

/*!
 * A WorkerPoolJob for solving mazezams which finds a solution as fast as possible.
 */
class MazezamSolverJobFastest : public MazezamSolverJob<ConfigEqv>
{
public:
	MazezamSolverJobFastest(const MazezamData& m) : MazezamSolverJob<ConfigEqv>(m, mzmslv::DEPTH_FIRST) {}
	virtual MazezamSolutionType getType() const { return MAZEZAM_SOLUTION_FASTEST; }
};


// TEMPLATE IMPLEMENTATIONS


template<class C>
MazezamSolverJob<C>::MazezamSolverJob(const MazezamData& m, mzmslv::SearchType type)
: MazezamData(m)
, mzmslv::SolverJob<C>(type, new C(*this))
{
}

template<class C>
void MazezamSolverJob<C>::addToMzmPath(mzm_path& path) const
{
	assert(mzmslv::SolverJob<C>::m_solverResult == mzmslv::FOUND_SOLUTION);
	assert(mzmslv::SolverJob<C>::m_solutionPath.size() > 0);
	const C* init = mzmslv::SolverJob<C>::m_solutionPath[0];
	const MazezamData& level = init->m_level; 
	mzm_coord x = 0;
	mzm_coord y = level.getStart();
	init->addInitialMoves(path,x,y);
	for(unsigned int i = 0; i < mzmslv::SolverJob<C>::m_solutionPath.size() - 1; ++i)
		mzmslv::SolverJob<C>::m_solutionPath[i]->addMoves(*mzmslv::SolverJob<C>::m_solutionPath[i+1],path,x,y);
	// There is a final move "East" which the solver does not account for.
	mzmslv::SolverJob<C>::m_solutionPath.back()->addFinalMoves(path,x,y);
}

template<class C>
bool MazezamSolverJob<C>::isSolvable() const
{
	return mzmslv::SolverJob<C>::m_solverResult == mzmslv::FOUND_SOLUTION;
}

template<class C>
float MazezamSolverJob<C>::getRating() const
{
	assert(mzmslv::SolverJob<C>::m_solutionPath.size() > 0);
	const float numPushes = mzmslv::SolverJob<C>::m_solutionPath.size() - 1;
	const float diag = sqrt((getWidth() * getWidth()) + (getHeight() * getHeight()));
	return numPushes / diag;
}

template<class C>
const MazezamData& MazezamSolverJob<C>::getLevel() const
{
	return *static_cast<const MazezamData*>(this);
}

} // namespace mzm

#endif /*MAZEZAMSOLVERJOB_H_*/
