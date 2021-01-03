/* ***************************************************************************
 * Mode.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef HELPPROVIDER_H_
#define HELPPROVIDER_H_

#include <string>

class CursesWindow;

/*!
 * An abstract class which supports the drawing of help windows. 
 */
class Mode
{
public:
	enum ModeReturnValue {
		//! Quit the application.
		QUIT,
		//! Exit the current mode (playback or playing).
		EXIT,
		//! Edit the current level.
		EDIT,
		//! Play the current level.
		PLAY,
		//! View a solution.
		SOLVE,
		//! Try to make the level more difficult.
		IMPROVE,
		//! Don't improve, but return to the last MazezaM edited.
		ABANDON,
		//! Keep on truckin'.
		LOOP
	};	
public:
	/*!
	 * Destructor.
	 */
	virtual ~Mode() {}
	/*!
	 * Return the name of the mode.
	 */
	virtual std::string getModeName() { return ""; }
	/*!
	 * Perform the processing appropriate to this mode.
	 * \param parent the Mode to which keys should first be passed.
	 */
	virtual ModeReturnValue modeLoop(CursesWindow& w, bool useColors, Mode* parent = NULL) = 0;
	/*!
	 * Handle the key k.
	 * \param w the ncurses window.
	 * \param k the key.
	 * \param next the mode to pass the key to if this made hasn't processed it.
	 */
	virtual ModeReturnValue processKey(CursesWindow& w, int k, Mode* next = NULL) = 0;
	/*!
	 * Draw help keys into the window at the given coordinates.
	 * \param w the ncurses window.
	 * \param y the y coordinate.
	 * \param x the x coordinate.
	 * \param next the mode which should draw help info after this.
	 */
	virtual void drawHelpInfo(CursesWindow& w, int y, int x, Mode* next = NULL) = 0; 
};

#endif /*HELPPROVIDER_H_*/
