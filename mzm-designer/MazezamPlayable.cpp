/* ***************************************************************************
 * MazezamPlayable.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <cassert>

#include "CursesWindow.hpp"
#include "MazezamPlayable.hpp"
#include "MazezamCurses.hpp"

MazezamPlayable::MazezamPlayable(const mzm::MazezamData& m)
: MazezamHistory(m)
{
}

MazezamPlayable::~MazezamPlayable()
{
}


Mode::ModeReturnValue MazezamPlayable::processKey(CursesWindow& w, int k, Mode* next)
{
	switch (k) {
		case 'x':
			return EXIT;
		case 'R':
			reset();
			break;
		case 'u':
			if (canUndo())
				undo();
			break;
		case 'r':
			if (canRedo())
				redo();
			break;
		case CursesWindow::KEY_UP_:
			if (canMoveNorth())
				moveNorth();
			break;
		case CursesWindow::KEY_DOWN_:
			if (canMoveSouth())
				moveSouth();
			break;
		case CursesWindow::KEY_RIGHT_:
			if (canMoveEast())
				moveEast();
			else if (canPushEast())
				pushEast();
			break;
		case CursesWindow::KEY_LEFT_:
			if (canMoveWest())
				moveWest();
			else if (canPushWest())
				pushWest();
			break;	
	}
	if (next)
		return next->processKey(w, k);
	else
		return LOOP;
}

Mode::ModeReturnValue MazezamPlayable::modeLoop(CursesWindow& w, bool useColors, Mode* parent )
{
	// Make cursor invisible.
  	CursesWindow::curs_set_(0);
	// This makes getch have blocking behaviour.
	w.nodelay_(false);
	MazezamCurses::draw(&w, useColors, *this);
	w.refresh_();
	// The delay in steps.
	ModeReturnValue ret = LOOP;
	do
	{
		MazezamCurses::draw(&w, useColors, *this);
		w.refresh_();
		ret = parent->processKey(w, w.getch_(), this);
	} while (ret == LOOP);
	return ret;
}

std::string MazezamPlayable::getModeName() {
	return "Playing";
}

struct KeyHelp 
{
	std::string key;
	std::string description;
};

KeyHelp MazezamPlayableHelpKeys[] = {
	{"x", "exit"},
	{"R", "reset"},
	{"u", "undo"},
	{"r", "redo"}
};

int numMazezamPlayableHelpKeys = sizeof(MazezamPlayableHelpKeys) / sizeof(KeyHelp);

void MazezamPlayable::drawHelpInfo(CursesWindow& w, int y, int x, Mode* next)
{
	for (int i = 0; i < numMazezamPlayableHelpKeys; ++i) {
		//wattron(w,A_UNDERLINE);
		w.mvprintw_(y+i,x,MazezamPlayableHelpKeys[i].key.c_str());
		//wattroff(w,A_UNDERLINE);
		w.printw_(" ");
		w.printw_(MazezamPlayableHelpKeys[i].description.c_str());
	};
}
