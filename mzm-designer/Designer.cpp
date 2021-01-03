/* ***************************************************************************
 * Designer.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "Designer.hpp"

#include <cassert>
#include <cstring>
#include <ctime>
#include <memory>

#include <mzm/MazezamSolver/MazezamSolutionType.hpp>
#include "CursesInit.hpp"
#include "MazezamPlayable.hpp"
#include "MazezamPlayback.hpp"
#include "Improver.hpp"
#include "MazezamCurses.hpp"

#define SOLUTION_WINDOW_HEIGHT 3
#define APPLICATION_TITLE "MazezaM Designer"
#define SOLUTION_DETAILS_OFFSET 17
#define HELP_WINDOW_WIDTH 20
#define DEFAULT_IMPROVER_RADIUS 5

// The number of characters wide info values are printed as. 
#define CHARS_FOR_INFO 5
#define CHARS_FOR_INFO_LESS_ONE 4
#define INT_INFO_ESCAPE_STRING(numchars) "%" STRINGIFY(numchars) "d"

using namespace mzmslv;

Designer::Designer(bool useColors, int numThreads, std::ostream& os, unsigned int saveNum)
: m_editor()
, m_workerPool(numThreads)
, m_backgroundSolver(m_workerPool, *this)
, m_outerWindow(CursesWindow::getOuterWindow())
, m_ostreamForSaving(os)
, m_saveNum(saveNum)
, m_improverRadius(DEFAULT_IMPROVER_RADIUS)
, m_hasBeenSaved(false)
, m_mode(EDITING)
{
	m_useColors = initTerminal(useColors) && useColors;
	setUpWindows();
	m_editor.setObserver(this);
}

Designer::Designer(bool useColors, int numThreads, std::ostream& os, const mzm::MazezamData& startLevel, unsigned int saveNum)
: m_editor(startLevel)
, m_workerPool(numThreads)
, m_backgroundSolver(m_workerPool, *this)
, m_outerWindow(CursesWindow::getOuterWindow())
, m_ostreamForSaving(os)
, m_saveNum(saveNum)
, m_improverRadius(DEFAULT_IMPROVER_RADIUS)
, m_hasBeenSaved(false)
, m_mode(EDITING)
{
	m_useColors = initTerminal(useColors) && useColors;
	setUpWindows();
	m_editor.setObserver(this);
	m_backgroundSolver.setNewLevel(startLevel, mzm::MAZEZAM_SOLUTION_FEWEST_PUSHES | mzm::MAZEZAM_SOLUTION_NUM_PUSHES | mzm::MAZEZAM_SOLUTION_NUM_MOVES | mzm::MAZEZAM_SOLUTION_RATING);
}

bool Designer::initTerminal(bool useColors)
{
	CursesWindow::cbreak_();
	// Obtain keypad codes
	//CursesWindow::keypad_(true);
	// Don't automatically echo characters
	CursesWindow::noecho_();
 	// If the system has color, then use it.
	if (useColors & CursesWindow::has_colors_()) {
		CursesWindow::start_color_();
		CursesWindow::init_pair_(MZM_ROW1, CursesWindow::COLOR_RED_, CursesWindow::COLOR_BLACK_);
		CursesWindow::init_pair_(MZM_ROW2, CursesWindow::COLOR_GREEN_, CursesWindow::COLOR_BLACK_);
		CursesWindow::init_pair_(MZM_ROW3, CursesWindow::COLOR_BLUE_, CursesWindow::COLOR_BLACK_);
		CursesWindow::init_pair_(MZM_ROW4, CursesWindow::COLOR_YELLOW_, CursesWindow::COLOR_BLACK_);
		CursesWindow::init_pair_(MZM_ROW5, CursesWindow::COLOR_MAGENTA_, CursesWindow::COLOR_BLACK_);
		CursesWindow::init_pair_(MZM_ROW6, CursesWindow::COLOR_CYAN_, CursesWindow::COLOR_BLACK_);
		CursesWindow::init_pair_(MZM_BORDER, CursesWindow::COLOR_WHITE_, CursesWindow::COLOR_BLACK_);
		CursesWindow::init_pair_(MZM_PERSON, CursesWindow::COLOR_WHITE_, CursesWindow::COLOR_BLACK_);
		return true;
	}
	return false;
}

void Designer::setUpWindows()
{
	int begx,begy;
	int maxy,maxx;
	m_outerWindow->getbegyx_(begy,begx);
	m_outerWindow->getmaxyx_(maxy,maxx);
	if ((maxx < 50) || (maxy < 15))
		throw WindowTooSmall();
	// Create the windows safely.
	{
		std::auto_ptr<CursesWindow> editorWindow(new CursesWindow(m_outerWindow, maxy - SOLUTION_WINDOW_HEIGHT - 3, maxx - HELP_WINDOW_WIDTH - 3, begy + 1, begx + HELP_WINDOW_WIDTH + 2));
		std::auto_ptr<CursesWindow> infoWindow(new CursesWindow(m_outerWindow, SOLUTION_WINDOW_HEIGHT, maxx - HELP_WINDOW_WIDTH - 3, begy + maxy - SOLUTION_WINDOW_HEIGHT - 1, begx + HELP_WINDOW_WIDTH + 3));
		std::auto_ptr<CursesWindow> helpWindow(new CursesWindow(m_outerWindow, maxy - 2, HELP_WINDOW_WIDTH, begy + 1, begx + 1));
		m_editorWindow = editorWindow.release();
		m_infoWindow = infoWindow.release();
		m_helpWindow = helpWindow.release();
	}
	m_outerWindow->attron_(CursesWindow::COLOR_PAIR_(NORMAL_COLORS));
	m_infoWindow->attron_(CursesWindow::COLOR_PAIR_(NORMAL_COLORS));
	m_helpWindow->attron_(CursesWindow::COLOR_PAIR_(NORMAL_COLORS));	
}

Designer::~Designer()
{
	// Just in case a background thread is still working, and subsequently
	// tries to draw to the deleted infoWindow.
	delete m_helpWindow;
	delete m_infoWindow;
	delete m_editorWindow;
	delete m_outerWindow;
}


void Designer::draw()
{
	int maxy,maxx;
	m_outerWindow->attron_(CursesWindow::COLOR_PAIR_(NORMAL_COLORS));
	m_outerWindow->getmaxyx_(maxy,maxx);
	// A box which surrounds the whole window.
	m_outerWindow->box_(0,0);
	// A dividing line between the help window and other windows.
	m_outerWindow->mvaddch_(0, HELP_WINDOW_WIDTH + 1, CursesWindow::ACS_TTEE_);
	m_outerWindow->mvvline_(1, HELP_WINDOW_WIDTH + 1, CursesWindow::ACS_VLINE_, maxy - 1);
	m_outerWindow->mvaddch_(maxy - 1, HELP_WINDOW_WIDTH + 1, CursesWindow::ACS_BTEE_);
	// A dividing line between the editor and solution windows.
	m_outerWindow->mvaddch_(maxy - SOLUTION_WINDOW_HEIGHT - 2, HELP_WINDOW_WIDTH + 1, CursesWindow::ACS_LTEE_);
	m_outerWindow->hline_(CursesWindow::ACS_HLINE_, maxx - HELP_WINDOW_WIDTH - 3);
	m_outerWindow->mvaddch_(maxy - SOLUTION_WINDOW_HEIGHT - 2, maxx - 1, CursesWindow::ACS_RTEE_);
	// The application title
	m_outerWindow->move_(0, (maxx - strlen(APPLICATION_TITLE)) / 2);
	m_outerWindow->printw_(" %s ",APPLICATION_TITLE);
	
	//
	m_outerWindow->refresh_();
	drawInfoHeaders();
	drawSaveNumInfo();
	drawRadiusInfo();
	m_infoWindow->refresh_();
}


void Designer::drawSolutionInfo()
{
	m_infoWindow->lock();
	int maxy,maxx;
	m_infoWindow->getmaxyx_(maxy,maxx);
	const unsigned int xoffset = (maxx / 2) + SOLUTION_DETAILS_OFFSET;

	if (!m_summary.m_isSolvable)
		m_infoWindow->attron_(CursesWindow::A_REVERSE_);
	if (m_summary.m_validSolutionFlags & mzm::MAZEZAM_SOLUTION_RATING)
		m_infoWindow->mvprintw_(0, xoffset, MAZEZAM_RATING_ESCAPE_STRING(CHARS_FOR_INFO), m_summary.m_rating);
	else
		m_infoWindow->mvhline_(0, xoffset, ' ', CHARS_FOR_INFO);
	if (m_summary.m_validSolutionFlags & mzm::MAZEZAM_SOLUTION_NUM_MOVES)
		m_infoWindow->mvprintw_(1, xoffset, INT_INFO_ESCAPE_STRING(CHARS_FOR_INFO), m_summary.m_numMoves);
	else
		m_infoWindow->mvhline_(1, xoffset, ' ', CHARS_FOR_INFO);
	if (m_summary.m_validSolutionFlags & mzm::MAZEZAM_SOLUTION_NUM_PUSHES)
		m_infoWindow->mvprintw_(2, xoffset, INT_INFO_ESCAPE_STRING(CHARS_FOR_INFO), m_summary.m_numPushes);
	else
		m_infoWindow->mvhline_(2, xoffset, ' ', CHARS_FOR_INFO);
	if (!m_summary.m_isSolvable)
		m_infoWindow->attroff_(CursesWindow::A_REVERSE_);

	m_infoWindow->refresh_();
	m_infoWindow->unlock();
	// The following returns the cursor to the editor window.
	m_editorWindow->refresh_();
}

void Designer::drawSaveNumInfo()
{
	m_infoWindow->lock();
	if (m_hasBeenSaved)
		m_infoWindow->mvprintw_(0,SOLUTION_DETAILS_OFFSET, INT_INFO_ESCAPE_STRING(CHARS_FOR_INFO), m_saveNum);
	else
		m_infoWindow->mvprintw_(0,SOLUTION_DETAILS_OFFSET, INT_INFO_ESCAPE_STRING(CHARS_FOR_INFO_LESS_ONE) "*", m_saveNum);
	m_infoWindow->unlock();
}

void Designer::drawRadiusInfo()
{
	m_infoWindow->lock();
	m_infoWindow->mvprintw_(1,SOLUTION_DETAILS_OFFSET, INT_INFO_ESCAPE_STRING(CHARS_FOR_INFO), m_improverRadius);
	m_infoWindow->unlock();
}

void Designer::drawInfoHeaders()
{
	m_infoWindow->lock();
	int maxy,maxx;
	m_infoWindow->getmaxyx_(maxy,maxx);
	m_infoWindow->mvprintw_(0,0,"Level number:");
	m_infoWindow->mvprintw_(1,0,"Improver radius:");
	//mvwprintw(m_infoWindow,1,0,"Solvable:");
	m_infoWindow->mvprintw_(0,maxx / 2,"Rating:");
	m_infoWindow->mvprintw_(1,maxx / 2,"# Moves:");
	m_infoWindow->mvprintw_(2,maxx / 2,"# Pushes:");
	m_infoWindow->unlock();
}


void Designer::update(Subject* subject)
{
	if (subject == &m_editor) {
		// The level has changed.
		if (m_hasBeenSaved) {
			++m_saveNum;
			m_hasBeenSaved = false;
			drawSaveNumInfo();
		}
		// Blank the solution window.
		m_summary.m_validSolutionFlags = 0;
		drawSolutionInfo();
		// Start solving the level.
		m_backgroundSolver.setNewLevel(m_editor.getLevel(), mzm::MAZEZAM_SOLUTION_FEWEST_PUSHES | mzm::MAZEZAM_SOLUTION_NUM_PUSHES | mzm::MAZEZAM_SOLUTION_NUM_MOVES | mzm::MAZEZAM_SOLUTION_RATING);
	} else
		assert(false);
}

Mode::ModeReturnValue Designer::processKey(CursesWindow& w, int k, Mode* next)
{
	switch(k) {
		// Common to all modes.
		case 'q':
			return QUIT;
		case 's':
			if (!m_hasBeenSaved)
				saveLevel();
			break;
		default: {
			// Keys meaningful while editing.
			if (m_mode == EDITING) {
				switch (k) {
					case 'p':
						return PLAY;
					case 'v':
						return SOLVE;
					case 'I':
						return IMPROVE;
					case '+':
						++m_improverRadius;
						drawRadiusInfo();
						m_infoWindow->refresh_();
						break;
					case '-':
						if (m_improverRadius > 1) {
							--m_improverRadius;
							drawRadiusInfo();
							m_infoWindow->refresh_();
						}
						break;
					default:
						break;
				}
			}
			break;
		}
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

KeyHelp DesignerHelpKeys[] = {
	{"q", "quit"},
	{"s", "save"},
	// Editing only
	{"-/+", "improver-/+"},
	{"p", "play"},
	{"v", "view"},
	{"I", "improve"}
};

int numDesignerHelpKeys = sizeof(DesignerHelpKeys) / sizeof(KeyHelp);

void Designer::drawHelpInfo(CursesWindow& w, int y, int x, Mode* next)
{
	const int numHelpKeys = m_mode == EDITING ? numDesignerHelpKeys : 2;
	for (int i = 0; i < numHelpKeys; ++i) {
		//wattron(w,A_UNDERLINE);
		w.mvprintw_(y + i,x,DesignerHelpKeys[i].key.c_str());
		//wattroff(w,A_UNDERLINE);
		w.printw_(" ");
		w.printw_(DesignerHelpKeys[i].description.c_str());
	}
	assert(next != NULL);
	next->drawHelpInfo(w, y + numHelpKeys, x, NULL);
}

void Designer::drawHelpWindow(Mode* next)
{
	m_helpWindow->clear_();
	int maxy,maxx;
	m_helpWindow->getmaxyx_(maxy,maxx);
	// The title
	m_helpWindow->attron_(CursesWindow::A_UNDERLINE_);
	const std::string& modeName = next->getModeName();
	m_helpWindow->mvprintw_(0,(maxx - modeName.length() ) / 2, modeName.c_str());
	m_helpWindow->attroff_(CursesWindow::A_UNDERLINE_);
	int y = 2;
	drawHelpInfo(*m_helpWindow, y, 1, next);
	m_helpWindow->refresh_();
}

void Designer::designLevel()
{
	modeLoop(*m_editorWindow, m_useColors);
}

Mode::ModeReturnValue Designer::modeLoop(CursesWindow& w, bool useColors, Mode* parent)
{
	draw();
	drawHelpWindow(&m_editor);
	ModeReturnValue ret = EDIT;
	while (ret != QUIT) {
		switch (ret) {
			case Mode::EDIT:
				m_mode = EDITING;
				drawHelpWindow(&m_editor);
				ret = m_editor.modeLoop(*m_editorWindow, m_useColors, this);
				break;
			case Mode::EXIT:
				// Exiting from any mode returns to the editor.
				ret = EDIT;
				break;
			case Mode::PLAY:
				{
					m_mode = PLAYING;
					MazezamPlayable mp(m_editor.getLevel());
					drawHelpWindow(&mp);
					ret = mp.modeLoop(*m_editorWindow, m_useColors, this);
				}
				break;
			case Mode::SOLVE:
				m_backgroundSolver.wait();
				// If we've found a solution
				if (m_summary.m_isSolvable) {
					m_mode = PLAYBACK;
					MazezamPlayback mp(m_editor.getLevel(), m_summary.m_solution);
					drawHelpWindow(&mp);
					ret = mp.modeLoop(*m_editorWindow, m_useColors, this);
				}
				break;
			case Mode::IMPROVE:
				{
					m_mode = IMPROVING;
					const mzm::Selection* selection = &m_editor.getSelection();
					if (!selection->hasSelectedPosition())
						selection = NULL;
					// Wait until the background solver has rated the current level.
					m_backgroundSolver.wait();
					unsigned int oldFlags = m_summary.m_validSolutionFlags;
					m_summary.m_validSolutionFlags &= ~mzm::MAZEZAM_SOLUTION_NUM_MOVES;

					drawSolutionInfo();
					Improver mi(m_editor.getLevel(), m_summary.m_rating, m_workerPool, *this, m_improverRadius, selection);
					drawHelpWindow(&mi);
					ret = mi.modeLoop(*m_editorWindow, m_useColors, this);
					if (ret == ABANDON)
					{
						m_summary.m_validSolutionFlags = oldFlags;
						ret = EDIT;
					}
					else if (ret == EXIT)
					{
						mzm::MazezamData m = mi.getBestLevel();
						// Improvement doesn't determine moves.
						m_editor.setLevel(m);
						m_backgroundSolver.setNewLevel(m, mzm::MAZEZAM_SOLUTION_NUM_MOVES );
						ret = EDIT;
					}
					drawSolutionInfo();
				}
				break;
			default:
				break;
		}
	}
	return QUIT;
	// Autosave if there's a unsaved level.
	//if (!m_hasBeenSaved)
	//	saveLevel(true);
}

void Designer::saveLevel(bool autoSave)
{
	m_ostreamForSaving << ";Number: " << m_saveNum << ( autoSave ? " (Autosave)" : "" ) << std::endl;
	
	// Get the current time.
	time_t rawtime;
	time ( &rawtime );
	tm *const timeinfo = localtime ( &rawtime );
	m_ostreamForSaving << ";Date: " << asctime(timeinfo) << std::endl;
	
	// Output the level 
	m_ostreamForSaving << m_editor.getLevel() << std::endl;
	
	
	// Update the info window.
	m_hasBeenSaved=true;
	drawSaveNumInfo();
}

void Designer::collectSolution(unsigned int levelNumber, mzm::MazezamSolutionType solutionType, const mzm::MazezamSolutionSource& source)
{
	m_summary.m_isSolvable = source.isSolvable();
	if (m_summary.m_isSolvable) {
		switch (solutionType) {
			case mzm::MAZEZAM_SOLUTION_FEWEST_PUSHES:
				m_summary.m_solution.clear();
				m_summary.m_validSolutionFlags |= mzm::MAZEZAM_SOLUTION_FEWEST_PUSHES;
				source.addToMzmPath(m_summary.m_solution);
				break;
			default:
				assert(false);
		}
	} else {
		m_summary.m_isSolvable = false;
		m_summary.m_rating = MAZEZAM_RATING_UNSOLVABLE;
		m_summary.m_validSolutionFlags = mzm::MAZEZAM_SOLUTION_ALL;
		m_summary.m_numMoves = 0;
		m_summary.m_numPushes = 0;
		drawSolutionInfo();
	}
}

void Designer::collectRating(unsigned int levelNumber, mzm::MazezamRating rating)
{
	m_summary.m_validSolutionFlags |= mzm::MAZEZAM_SOLUTION_RATING;
	m_summary.m_rating = rating;
	drawSolutionInfo();
}

void Designer::collectNumPushes(unsigned int levelNumber, unsigned int numPushes)
{
	m_summary.m_validSolutionFlags |= mzm::MAZEZAM_SOLUTION_NUM_PUSHES;
	m_summary.m_numPushes = numPushes;
	drawSolutionInfo();
}

void Designer::collectNumMoves(unsigned int levelNumber, unsigned int numMoves)
{
	m_summary.m_validSolutionFlags |= mzm::MAZEZAM_SOLUTION_NUM_MOVES;
	m_summary.m_numMoves = numMoves;
	drawSolutionInfo();
}

void Designer::collectImprovement(unsigned int levelNumber, const mzm::MazezamData& level)
{
	// If there's an improvement, then it's solvable.
	m_summary.m_isSolvable = true;
	MazezamCurses::draw(m_editorWindow, m_useColors, level);
}
