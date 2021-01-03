/* ***************************************************************************
 * OutstreamSolutionCollector.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef OUTFILESOLUTIONCOLLECTOR_H_
#define OUTFILESOLUTIONCOLLECTOR_H_

#include <mzm/MazezamSolver/SolutionCollector.hpp>
#include <list>

namespace mzm {

/*!
 * A solution collector which acts as a buffer to an ostream.
 * It buffers solutions so as to keep them in order.
 */
class OutstreamSolutionCollector : public SolutionCollector
{
public:
	OutstreamSolutionCollector(std::ostream& os, unsigned int solutionTypeFlags, bool copyMode);
	
	// SolutionCollector interface.
	virtual void collectLevelNumber(unsigned int levelNumber);
	virtual void collectSolution(unsigned int levelNumber, MazezamSolutionType solutionType, const MazezamSolutionSource& source);
	virtual void collectRating(unsigned int levelNumber, MazezamRating rating, unsigned int numPushes);
private:
	/*!
	 * The stream to which this object is writing.
	 */
	std::ostream& m_outStream;
	/*!
	 * A bitfield indicating the type of solutions which are desired.
	 */
	unsigned int m_solutionTypeFlags;
	/*!
	 * If in copyMode, then we must output the solutions to one level before
	 * we read the next one. Naturally, this limits concurrency.
	 */
	bool m_copyMode;
	/*!
	 * The ordered sequence of level numbers for which solutions are expected.
	 */
	std::list<unsigned int> m_levelNumbers;
	/*!
	 * A solution.
	 */
	struct Solution {
		MazezamSolutionType m_solutionType;
		mzm_path m_solution;
	};
	/*!
	 * The data being held for a level which hasn't been finished yet.
	 */
	struct PerLevelOutput {
		unsigned int m_levelNumber;
		unsigned int m_outstandingFlags;
		MazezamRating m_rating;
		std::vector<Solution> m_solutions;
	};
	/*!
	 * The information collected, but yet to be written out. 
	 */
	std::list<PerLevelOutput> m_bufferedOutput;
private:
	/*!
	 * Find the matching per level output or create one.
	 */
	PerLevelOutput& findOutput(unsigned int levelNumber);
	/*!
	 * Flush any output which is finished. 
	 */
	void postCollectCheck();
	/*!
	 * Write the output 
	 */
	void writeOutput(const PerLevelOutput& output) const;
};

} // namespace mzm

#endif /*OUTFILESOLUTIONCOLLECTOR_H_*/
