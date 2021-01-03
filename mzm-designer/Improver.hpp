/* ***************************************************************************
 * Improver.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef IMPROVER_H_
#define IMPROVER_H_

#include <mzm/MazezamSolver/MazezamImprover.hpp>
#include "Mode.hpp"

/*!
 * The mode when a mazezam is being improved.
 */
class Improver : public Mode
{
public:
	/*!
	 * Constructor.
	 */
	Improver(const mzm::MazezamData& m, mzm::MazezamRating startRating, mzmslv::WorkerPool& workerPool, mzm::SolutionCollector& collector, unsigned int radius, const mzm::Selection* selection);
	/*!
	 * Get the best level. 
	 */
	const mzm::MazezamData& getBestLevel() const;
	
	// Mode interface methods.
	virtual std::string getModeName();
	virtual ModeReturnValue modeLoop(CursesWindow& w, bool useColors, Mode* parent);
	virtual ModeReturnValue processKey(CursesWindow& w, int k, Mode* next = NULL);
	virtual void drawHelpInfo(CursesWindow& w, int y, int x, Mode* next);
public:
	/*!
	 * The MazezamImprover object.
	 */
	mzm::MazezamImprover m_improver;
};

#endif /*IMPROVER_H_*/
