/* ***************************************************************************
 * SolverJob.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef SOLVERJOB_H_
#define SOLVERJOB_H_

#include "WorkerPoolJob.hpp"
#include "Solver.hpp"

namespace mzmslv {

/*!
 * A workerPoolJob for solving problems with configurations of type C.
 */
template<class C>
class SolverJob : public WorkerPoolJob
{
public:
	/*!
	 * Create a SolverJob.
	 * \param searchType the type of search to use.
	 * \param init the initial configuration.
	 */
	SolverJob(SearchType searchType, C* init);
	
	// WorkerPoolJob interface.
	virtual Outcome doJob();
	virtual void stop();
	
	/*!
	 * Deletes the solution path, but not the initial configuration.
	 */
	virtual ~SolverJob();
	
	
	/*!
	 * Gets the result of the job.
	 */
	inline SolverResult getResult() const;
	/*!
	 * Gets the solution path.
	 */
	inline const typename Solver<C>::path& getPath() const;
protected:
	/*!
	 * The solver object which does the solving. 
	 */
	Solver<C> m_solver;

	//
	// Input
	//
	
	/*!
	 * What type of search should the solver use.
	 */
	SearchType m_searchType;
	/*!
	 * The initial configuration.
	 */
	C* m_initConfig;
	
	//
	// Output
	//

	/*!
	 * What the solver returned.
	 */	
	SolverResult m_solverResult;

	/*!
	 * The solution path.
	 */
	typename Solver<C>::path m_solutionPath;
};

// 
// Template implementation.
//

template<class C>
SolverJob<C>::SolverJob(SearchType searchType, C* init)
: m_searchType(searchType)
, m_initConfig(init)
{
}

template<class C>
WorkerPoolJob::Outcome SolverJob<C>::doJob()
{
	m_solverResult = m_solver.findSolution(m_searchType, m_initConfig, m_solutionPath);
	return (m_solverResult != INTERRUPTED) ? JOB_FINISHED : JOB_INTERRUPTED;
}

template<class C>
void SolverJob<C>::stop()
{
	m_solver.stop();
}

template<class C>
SolverJob<C>::~SolverJob()
{
	Solver<C>::deletePath(m_solutionPath);
}

template<class C>
SolverResult SolverJob<C>::getResult() const
{
	return m_solverResult;
}

template<class C>
const typename Solver<C>::path& SolverJob<C>::getPath() const
{
	return m_solutionPath;
}

} // namespace mzmslv

#endif /*SOLVERJOB_H_*/
