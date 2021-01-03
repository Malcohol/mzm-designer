/* ***************************************************************************
 * MazezamPlayback.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "MazezamPlayback.hpp"

#include <cassert>

#include "CursesWindow.hpp"
#include "MazezamCurses.hpp"

const int DELAY_DEFAULT = 200;
const int DELAY_MIN = 10;
// This value is taken from a ncurses manpage.
const int DELAY_MAX = 25500 / 2;


MazezamPlayback::MazezamPlayback(const mzm::MazezamData& m, const mzm::mzm_path& p) 
: MazezamHistory(m, p, 0)
, m_paused(false)
, m_delay(DELAY_DEFAULT)
{
}

MazezamPlayback::~MazezamPlayback()
{
}


Mode::ModeReturnValue MazezamPlayback::processKey(CursesWindow& w, int k, Mode* next)
{
	switch(k) {
		case 'i':
			if (m_delay > DELAY_MIN)
				m_delay = m_delay / 2;
			w.timeout_(m_delay);
			break;
		case 'd':
			if (m_delay < DELAY_MAX)
				m_delay = m_delay * 2;
			w.timeout_(m_delay);
			break;
		case 'p':
			if (m_paused)
				w.timeout_(m_delay);
			else
				w.nodelay_(false);
			m_paused = !m_paused;
			break;
		case ' ':
			if (canRedo()) {
				redo();
			}
			break;
		case 'b':
			if (canUndo()) {
				m_paused = true;
				w.nodelay_(false);
				undo();
				w.refresh_();
			}
			break;
		case 'x':
			return EXIT;
		case CursesWindow::ERR_: // Timeout
			if (canRedo()) {
				redo();
			}
			break;
		case 'r':
			if (canUndo()) {
				m_paused = true;
				w.nodelay_(false);
				reset();
			}
			break;
	}
	if (next)
		return next->processKey(w, k);
	else
		return LOOP;
}

Mode::ModeReturnValue MazezamPlayback::modeLoop(CursesWindow& w, bool useColors, Mode* parent)
{
	// Make cursor invisible.
  	CursesWindow::curs_set_(0);
	MazezamCurses::draw(&w, useColors, *this);
	w.refresh_();
	// The loop delay is a getch with a timeout.
	w.timeout_(m_delay);
	ModeReturnValue ret = LOOP;
	do
	{
		if (!canRedo())
			w.nodelay_(false);
		MazezamCurses::draw(&w, useColors, *this);
		w.refresh_();
		ret = parent->processKey(w, w.getch_(), this);
	} while (ret == LOOP);
	return ret;
}

std::string MazezamPlayback::getModeName() {
	return "Viewing";
}

struct KeyHelp 
{
	std::string key;
	std::string description;
};

KeyHelp MazezamPlaybackHelpKeys[] = {
	{"x", "exit"},
	{"r", "reset"},
	{"p", "pause/play"},
	{"SPACE", "next"},
	{"b", "back"},
	{"i", "increase speed"},
	{"d", "decrease speed"},
};

int numMazezamPlaybackHelpKeys = sizeof(MazezamPlaybackHelpKeys) / sizeof(KeyHelp);

void MazezamPlayback::drawHelpInfo(CursesWindow& w, int y, int x, Mode* next)
{
	for (int i = 0; i < numMazezamPlaybackHelpKeys; ++i) {
		//wattron(w,A_UNDERLINE);
		w.mvprintw_(y + i,x,MazezamPlaybackHelpKeys[i].key.c_str());
		//wattroff(w,A_UNDERLINE);
		w.printw_(" ");
		w.printw_(MazezamPlaybackHelpKeys[i].description.c_str());
	}
}
