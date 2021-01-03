/* ***************************************************************************
 * MazezamCurses.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "MazezamCurses.hpp"

#include <mzm/MazezamSolver/Selection.hpp>

void MazezamCurses::drawRow(CursesWindow* w, bool useColors, const mzm::MazezamData& m, mzm::mzm_coord r, const mzm::Selection* selection)
{
	if (useColors) w->attron_(CursesWindow::COLOR_PAIR_((r % NUM_ROW_COLORS) + 1));
	// print rows.
	for (int i = 0; i < m.getWidth(); ++i) {
		if (selection && selection->isSelected(r, i))
			w->attron_(CursesWindow::A_UNDERLINE_);
		else
			w->attroff_(CursesWindow::A_UNDERLINE_);
		if (m.getBlock(r,i))
			w->addch_(mzm::CHAR_BLOCK);
		else
			w->addch_(mzm::CHAR_SPACE);
	}
	w->attroff_(CursesWindow::A_UNDERLINE_);	
}

void MazezamCurses::draw(CursesWindow* w, bool useColors, const mzm::MazezamData& m, const mzm::Selection* selection)
{
	int maxy,maxx;
	// Get the dimensions of the window.
	w->getmaxyx_(maxy,maxx);
	int yoffset = (maxy - 1 - m.getHeight()) / 2;
	int xoffset = (maxx - 1 - m.getWidth()) / 2;
	// north wall
	if (useColors) w->attron_(CursesWindow::COLOR_PAIR_(MZM_BORDER));
	w->mvhline_(yoffset, xoffset, mzm::CHAR_WALL, m.getWidth()+2);
	for (int j = 0; j < m.getHeight(); ++j) {
		// west wall
		w->move_(yoffset+j+1, xoffset);
		w->addch_((j == m.getStart())? mzm::CHAR_ENTRANCE: mzm::CHAR_WALL);
		drawRow(w, useColors, m, j, selection);
		// east wall
		if (useColors) w->attron_(CursesWindow::COLOR_PAIR_(MZM_BORDER));
		w->addch_((j == m.getFinish())? mzm::CHAR_EXIT: mzm::CHAR_WALL);
	}
	// south wall
	if (useColors) w->attron_(CursesWindow::COLOR_PAIR_(MZM_BORDER));
	w->mvhline_(yoffset + m.getHeight() + 1, xoffset, mzm::CHAR_WALL, m.getWidth()+2);
}


void MazezamCurses::draw(CursesWindow* w, bool useColors, const mzm::MazezamPerson& m )
{
	draw(w, useColors, static_cast<const mzm::MazezamData&>(m), NULL);
	int maxy,maxx;
	// Get the dimensions of the window.
	w->getmaxyx_(maxy,maxx);
	int yoffset = (maxy - 1 - m.getHeight()) / 2;
	int xoffset = (maxx - 1 - m.getWidth()) / 2;
	w->attron_(CursesWindow::COLOR_PAIR_(MZM_PERSON));
	w->mvaddch_(yoffset + m.getY() + 1, xoffset + m.getX() + 1, mzm::CHAR_PERSON);
}

void MazezamCurses::draw(CursesWindow* w, bool useColors, const Editor& e)
{
	const mzm::MazezamData& m = e.getLevel();
	draw(w, useColors, m, &e.getSelection());
	int maxy,maxx;
	// Get the dimensions of the window.
	w->getmaxyx_(maxy,maxx);
	int yoffset = (maxy - 1 - m.getHeight()) / 2;
	int xoffset = (maxx - 1 - m.getWidth()) / 2;
	// Cursor
	w->move_(yoffset + e.getCursY() + 1, xoffset + e.getCursX() + 1);
}
