/* ***************************************************************************
 * MazezamCurses.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMCURSES_H_
#define MAZEZAMCURSES_H_

#include <mzm/Mazezam/MazezamPerson.hpp>
#include "Editor.hpp"
#include "CursesInit.hpp"
#include "CursesWindow.hpp"

/*!
 * A collection of static functions which can be used to draw Mazezams using
 * curses.
 */
class MazezamCurses
{
public:
	/*!
	 * Draw the level into an ncurses window.
	 * \param w the window pointer.
	 */
	static void draw(CursesWindow* w, bool useColors, const mzm::MazezamData& m, const mzm::Selection* selection = NULL);
	static void draw(CursesWindow* w, bool useColors, const mzm::MazezamPerson& m);
	static void draw(CursesWindow* w, bool useColors, const Editor& e);
	
	/*!
	 * Draw the interior of a row.
	 * \param w the window pointer.
	 * \param r the row.
	*/
	static void drawRow(CursesWindow* w, bool useColors, const mzm::MazezamData& m, mzm::mzm_coord r, const mzm::Selection* selection = NULL);
};

#endif /*MAZEZAMCURSES_H_*/
