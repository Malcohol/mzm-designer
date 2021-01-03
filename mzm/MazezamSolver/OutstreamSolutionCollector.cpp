/* ***************************************************************************
 * OutstreamSolutionCollector.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <mzm/MazezamSolver/OutstreamSolutionCollector.hpp>

#include <cassert>

namespace mzm {

OutstreamSolutionCollector::OutstreamSolutionCollector(std::ostream& os, unsigned int solutionTypeFlags, bool copyMode)
: m_outStream(os)
, m_solutionTypeFlags(solutionTypeFlags)
, m_copyMode(copyMode)
{
}

void OutstreamSolutionCollector::collectLevelNumber(unsigned int levelNumber)
{
	m_levelNumbers.push_back(levelNumber);
}

void OutstreamSolutionCollector::collectSolution(unsigned int levelNumber, MazezamSolutionType solutionType, const MazezamSolutionSource& source)
{
	if (source.isSolvable()) {
		assert(solutionType & m_solutionTypeFlags);
		PerLevelOutput& output = findOutput(levelNumber);
		// Check we haven't already seen this solution?
		assert(solutionType & output.m_outstandingFlags);
		output.m_outstandingFlags &= ~solutionType;
		Solution solution;
		solution.m_solutionType = solutionType;
		source.addToMzmPath(solution.m_solution);
		output.m_solutions.push_back(solution);
	} else {
		PerLevelOutput& output = findOutput(levelNumber);
		output.m_outstandingFlags &= ~solutionType;
		output.m_rating = MAZEZAM_RATING_UNSOLVABLE;
	}
	postCollectCheck();
}

void OutstreamSolutionCollector::collectRating(unsigned int levelNumber, MazezamRating rating, unsigned int numPushes)
{
	PerLevelOutput& output = findOutput(levelNumber);
	output.m_outstandingFlags &= ~MAZEZAM_SOLUTION_RATING;
	output.m_rating = rating;
	
	postCollectCheck();
}

OutstreamSolutionCollector::PerLevelOutput& OutstreamSolutionCollector::findOutput(unsigned int levelNumber)
{
	std::list<PerLevelOutput>::iterator iterator = m_bufferedOutput.begin();
	// Find the position the output should go.
	while (iterator != m_bufferedOutput.end()) {
		if (levelNumber == iterator->m_levelNumber)
			// There was an existing entry, so return it.
			return *iterator;
		else if (levelNumber < iterator->m_levelNumber)
			break;
		++iterator;
	}
	// Create a new output.
	PerLevelOutput output;
	output.m_levelNumber = levelNumber;
	output.m_outstandingFlags = m_solutionTypeFlags;
	return *m_bufferedOutput.insert(iterator, output);
}

void OutstreamSolutionCollector::postCollectCheck()
{
	// We've just collected some solutions, so there must be both buffered output and levelNumbers.
	assert(m_bufferedOutput.size() > 0);
	assert(m_levelNumbers.size() > 0);
	
	// There may be levels for which no solutions have been collected yet,
	assert(m_bufferedOutput.size() <= m_levelNumbers.size());
	
	std::list<PerLevelOutput>::iterator oi = m_bufferedOutput.begin();
	std::list<unsigned int>::iterator li = m_levelNumbers.begin();
	
	while ( (oi != m_bufferedOutput.end()) 
			&& (oi->m_outstandingFlags == 0)
			&& (oi->m_levelNumber == *li) ) {
		writeOutput(*oi);
		++oi;
		++li;
	}
	
	m_bufferedOutput.erase(m_bufferedOutput.begin(), oi);
	m_levelNumbers.erase(m_levelNumbers.begin(), li);
}

void OutstreamSolutionCollector::writeOutput(const PerLevelOutput& output) const
{
	const int numSolutions = output.m_solutions.size();
	if (!m_copyMode)
		m_outStream << "Level " << output.m_levelNumber << "\n";

	if (numSolutions) {
		for (int i = 0; i < numSolutions; ++i) {
			if (m_copyMode)
				m_outStream << ";;";
			const Solution& solution = output.m_solutions[i];
			switch (solution.m_solutionType) {
				case MAZEZAM_SOLUTION_FEWEST_PUSHES:
					if (m_solutionTypeFlags && solution.m_solutionType)
						m_outStream << "Solution(Pushes): " << solution.m_solution << "\n";
					break;
				case MAZEZAM_SOLUTION_FEWEST_MOVES:
					m_outStream << "Solution(Moves): " << solution.m_solution << "\n";
					break;
				case MAZEZAM_SOLUTION_FASTEST:
					m_outStream << "Solution: " << solution.m_solution << "\n";
					break;
				default:
					assert(false);
			}
		}
	}
	
	if (m_solutionTypeFlags & MAZEZAM_SOLUTION_RATING) {
		if (m_copyMode)
			m_outStream << ";;";				
		m_outStream << "Rating: " << output.m_rating << "\n";
	} else if (numSolutions == 0)
		m_outStream << "No solution\n";
}

} // namespace mzm
