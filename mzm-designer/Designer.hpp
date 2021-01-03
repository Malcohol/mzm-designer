/* ***************************************************************************
 * Designer.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef DESIGNER_H_
#define DESIGNER_H_

#include <ostream>

#include <mzm/MazezamSolver/BackgroundSolver.hpp>
#include <mzmcommon/Observer/Observer.hpp>
#include <mzmslv/WorkerPool.hpp>
#include <mzm/MazezamSolver/SolutionCollector.hpp>
#include <mzm/MazezamSolver/MazezamSolutionType.hpp>
#include "Editor.hpp"
#include "MazezamSolverSummary.hpp"
#include "CursesWindow.hpp"

/*!
 * Manages the editor and BackgroundSolver.
 * It observes changes to the state of the level in the editor.
 * It receives solutions from the background solver.
 */
class Designer : public Observer, public mzm::SolutionCollector, public Mode
{
public:
	/*!
	 * Exception thrown when the window isn't big enough to display all the data.
	 */
	class WindowTooSmall { };
	/*!
	 * Exception thrown when there is a ncurses error.
	 */
	class NCursesError { };
	/*!
	 * Constructor.
	 * \param numThreads the number of extra solverThreads to use (must be 1 or greater).
	 * \param os if non-null, then an ostream to use for saving.
	 * \param saveNum the number which the next level added to the save file would have.
	 */
	Designer(bool useColor, int numThreads, std::ostream& os, unsigned int saveNum = 1);
	/*!
	 * Constructor.
	 * \param numThreads the number of extra solverThreads to use (must be 1 or greater).
	 * \param os if non-null, then an ostream to use for saving.
	 * \param startLevel the level which should be opened at start.
	 * \param saveNum the number which the next level added to the save file would have.
	 */
	Designer(bool useColor, int numThreads, std::ostream& os, const mzm::MazezamData& startLevel, unsigned int saveNum = 1);
	/*!
	 * Destructor.
	 */
	virtual ~Designer();
	/*!
	 * The entry point for the design process.
	 */
	void designLevel();
	
	// Observer interface methods.
	virtual void update(Subject* subject);
	
	// SolutionCollector interface methods.
	virtual void collectSolution(unsigned int levelNumber, mzm::MazezamSolutionType m_solutionType, const mzm::MazezamSolutionSource& source);
	virtual void collectRating(unsigned int levelNumber, mzm::MazezamRating rating);
	virtual void collectNumPushes(unsigned int levelNumber, unsigned int numPushes);
	virtual void collectNumMoves(unsigned int levelNumber, unsigned int numMoves);
	virtual void collectImprovement(unsigned int levelNumber, const mzm::MazezamData& level);

	// Mode interface methods. 
	virtual ModeReturnValue modeLoop(CursesWindow& w, bool useColors, Mode* parent = NULL);
	virtual ModeReturnValue processKey(CursesWindow& w, int k, Mode* next = NULL);
	virtual void drawHelpInfo(CursesWindow& w, int y, int x, Mode* next = NULL);
private:
	/*!
	 * The editor.
	 */
	Editor m_editor;
	/*!
	 * A pool of worker threads.
	 */
	mzmslv::WorkerPool m_workerPool;
	/*!
	 * The solver.
	 */
	mzm::BackgroundSolver m_backgroundSolver;
	/*!
	 * Outer window (not owned by this object).
	 */
	CursesWindow* m_outerWindow;
	/*!
	 * The window into which the editor draws.
	 */
	CursesWindow* m_editorWindow;
	/*!
	 * The window into which information about the level are drawn.
	 */
	CursesWindow* m_infoWindow;
	/*!
	 * The window into which the key information is written.
	 */
	CursesWindow* m_helpWindow;
	/*!
	 * Whether colors should be used. 
	 */
	bool m_useColors;
	/*!
	 * A summary of the solution to the current level.
	 */
	MazezamSolverSummary m_summary;
	/*!
	 * The ostream to which levels should be saved.
	 */
	std::ostream& m_ostreamForSaving;
	/*!
	 * The number that the next level in the ostreamForSaving should be given.
	 */
	unsigned int m_saveNum;
	/*!
	 * The radius used by the improver.
	 */
	unsigned int m_improverRadius;
	/*!
	 * Notes whether the current level has been saved yet.
	 */
	bool m_hasBeenSaved;
	/*!
	 * The modes the designer can be in.
	 */
	enum ModeState {
		EDITING,
		PLAYING,
		PLAYBACK,
		IMPROVING
	};
	/*!
	 * The current mode.
	 */
	ModeState m_mode;
private:
	/*!
	 * Initialize the curses terminal.
	 */
	static bool initTerminal(bool useColors);
	/*!
	 * Set up the curses windows.
	 */
	void setUpWindows();
	/*!
	 * Draw the designer screen.
	 */
	void draw();
	/*!
	 * Draw the solution details in the info window.
	 */
	void drawSolutionInfo();
	/*!
	 * Draw the solution headers.
	 */
	void drawInfoHeaders();
	/*!
	 * Draw the save number in the info window.
	 */
	void drawSaveNumInfo();
	/*!
	 * Draw the improver radius in the info window.
	 */
	void drawRadiusInfo();
	/*!
	 * Write the level to the ostreamForSaving.
	 */
	void saveLevel(bool autoSave = false);
	/*!
	 * Draw the help window.
	 */
	void drawHelpWindow(Mode* next);
};

#endif /*DESIGNER_H_*/
