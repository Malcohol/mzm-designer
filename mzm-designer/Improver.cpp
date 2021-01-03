/* ***************************************************************************
 * Improver.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "Improver.hpp"
#include "MazezamCurses.hpp"

#include <mzm/MazezamSolver/SolutionCollector.hpp>
#include <mzm/MazezamSolver/Selection.hpp>

#define KEY_CHECK_DELAY 500

Improver::Improver(const mzm::MazezamData& m, mzm::MazezamRating startRating, mzmslv::WorkerPool& workerPool, mzm::SolutionCollector& collector, unsigned int radius, const mzm::Selection* selection)
: m_improver(m, startRating, workerPool, collector, radius, selection)
{
}

std::string Improver::getModeName()
{
	return "Improving";
}

Mode::ModeReturnValue Improver::modeLoop(CursesWindow& w, bool useColors, Mode* parent)
{
	// Make cursor invisible.
  	CursesWindow::curs_set_(0);
	//MazezamCurses::draw(&w, useColors, m_bestLevel);
	w.refresh_();
	// The loop delay is a getch with a timeout.
	w.nodelay_(false);
	w.timeout_(KEY_CHECK_DELAY);
	m_improver.improveAsynchronously();
	ModeReturnValue ret = LOOP;
	do
	{
		//MazezamCurses::draw(&w, useColors, m_improver.getBestLevel());
		w.refresh_();
		if (m_improver.isFinished())
			ret = EXIT;
		else
			ret = parent->processKey(w, w.getch_(), this);
	} while (ret == LOOP);
	m_improver.stop();
	return ret;
}

Mode::ModeReturnValue Improver::processKey(CursesWindow& w, int k, Mode* next)
{
	switch (k) {
		case 'x':
			m_improver.stop();
			return EXIT;
			break;
		case 'a':
			m_improver.stop();
			return ABANDON;
			break;
		default:
			break;
	}
	if (next)
		return next->processKey(w, k);
	else
		return LOOP;
}

struct KeyHelp 
{
	std::string key;
	std::string description;
};

KeyHelp improverHelpKeys[] = {
	{"x", "exit"},
	{"a", "abandon"}
};

int numImproverHelpKeys = sizeof(improverHelpKeys) / sizeof(KeyHelp);

void Improver::drawHelpInfo(CursesWindow& w, int y, int x, Mode* next)
{
	int maxx, maxy;
	w.getmaxyx_(maxy,maxx);
	const int numHelpKeys = std::min(numImproverHelpKeys, maxy-3); 
	for (int i = 0; i < numHelpKeys; ++i) {
		//wattron(w,A_UNDERLINE);
		w.mvprintw_(y + i,x,improverHelpKeys[i].key.c_str());
		//wattroff(w,A_UNDERLINE);
		w.printw_(" ");
		w.printw_(improverHelpKeys[i].description.c_str());
	}
}

const mzm::MazezamData& Improver::getBestLevel() const
{
	return m_improver.getBestLevel();
}

