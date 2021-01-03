/* ***************************************************************************
 * CursesWindow.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "CursesWindow.hpp"

#include <curses.h>

CursesWindow* CursesWindow::m_outerWindow = NULL;

#define CONSTANT_DEFINE(X) const int CursesWindow::X##_ = X;

CONSTANT_DEFINE(A_UNDERLINE)
CONSTANT_DEFINE(A_REVERSE)
CONSTANT_DEFINE(COLOR_BLACK)
CONSTANT_DEFINE(COLOR_RED)
CONSTANT_DEFINE(COLOR_GREEN)
CONSTANT_DEFINE(COLOR_YELLOW)
CONSTANT_DEFINE(COLOR_BLUE)
CONSTANT_DEFINE(COLOR_MAGENTA)
CONSTANT_DEFINE(COLOR_CYAN)
CONSTANT_DEFINE(COLOR_WHITE)

#undef CONSTANT_DEFINE

#define STATIC_DEFINE(X) int CursesWindow::X##_;

STATIC_DEFINE(ACS_LTEE)
STATIC_DEFINE(ACS_RTEE)
STATIC_DEFINE(ACS_TTEE)
STATIC_DEFINE(ACS_BTEE)
STATIC_DEFINE(ACS_HLINE)
STATIC_DEFINE(ACS_VLINE)

#undef STATIC_DEFINE

#define KEY_ENTRY(X) { X, CursesWindow::X##_ },

static const int keyTable[CursesWindow::NUM_KEYS][2] = {
	KEY_ENTRY(KEY_UP)
	KEY_ENTRY(KEY_DOWN)
	KEY_ENTRY(KEY_LEFT)
	KEY_ENTRY(KEY_RIGHT)
	KEY_ENTRY(KEY_HOME)
	KEY_ENTRY(KEY_END)
	KEY_ENTRY(KEY_DC)
	KEY_ENTRY(KEY_BACKSPACE)
	KEY_ENTRY(ERR)
};

#undef KEY_ENTRY

#define M_WINDOW static_cast<WINDOW*>(m_window)
#define PARENTS_WINDOW static_cast<WINDOW*>(m_parent->m_window)

CursesWindow* CursesWindow::getOuterWindow()
{
	if (!m_outerWindow)
	{
		OutputLock::create();
		InputLock::create();
		m_outerWindow = new CursesWindow();
	}
	return m_outerWindow;
}

#define STATIC_ASSIGN(X) X##_ = X;

CursesWindow::CursesWindow()
: m_parent(NULL)
{
	initscr();
	STATIC_ASSIGN(ACS_LTEE)
	STATIC_ASSIGN(ACS_RTEE)
	STATIC_ASSIGN(ACS_TTEE)
	STATIC_ASSIGN(ACS_BTEE)
	STATIC_ASSIGN(ACS_HLINE)
	STATIC_ASSIGN(ACS_VLINE)
	m_window = stdscr;
	pthread_mutex_init(&m_windowLock,NULL);
}

#undef STATIC_ASSIGN

CursesWindow::~CursesWindow()
{
	pthread_mutex_destroy(&m_windowLock);
	if (m_parent == NULL) {
		OutputLock::destroy();
		endwin();
	} else {
		delwin (M_WINDOW);
	}
}

CursesWindow::CursesWindow(CursesWindow* parentWindow, int height, int width, int y, int x)
: m_parent(parentWindow)
, m_window(subwin(PARENTS_WINDOW, height, width, y, x))
{
	if (!m_window)
		throw CursesError();
	pthread_mutex_init(&m_windowLock,NULL);
}

void CursesWindow::lock()
{
	pthread_mutex_lock(&m_windowLock);
}

void CursesWindow::unlock()
{
	pthread_mutex_unlock(&m_windowLock);
}

void CursesWindow::getmaxyx_(int& y, int& x) const
{
	OutputLock lock;
	getmaxyx(M_WINDOW,y,x);
}

void CursesWindow::getbegyx_(int& y, int& x) const
{
	OutputLock lock;
	getbegyx(M_WINDOW,y,x);
}

void CursesWindow::box_(int verch, int horch)
{
	OutputLock lock;
	if (box(M_WINDOW, verch, horch) == ERR)
		throw CursesError();
}

void CursesWindow::hline_(int ch, int n)
{
	OutputLock lock;
	if (whline(M_WINDOW, ch, n) == ERR)
		throw CursesError();
}

void CursesWindow::addch_(int ch)
{
	OutputLock lock;
	if (waddch(M_WINDOW, ch) == ERR)
		throw CursesError();
}

void CursesWindow::mvaddch_(int y, int x, int ch)
{
	OutputLock lock;
	if (mvwaddch(M_WINDOW, y, x, ch) == ERR)
		throw CursesError();
}

void CursesWindow::mvvline_(int y, int x, int ch, int n)
{
	OutputLock lock;
	if (mvwvline(M_WINDOW, y, x, ch, n) == ERR)
		throw CursesError();	
}

void CursesWindow::mvhline_(int y, int x, int ch, int n)
{
	OutputLock lock;
	if (mvwhline(M_WINDOW, y, x, ch, n) == ERR)
		throw CursesError();	
}

void CursesWindow::move_(int y, int x)
{
	OutputLock lock;
	if (wmove(M_WINDOW, y, x) == ERR)
		throw CursesError();
}

void CursesWindow::refresh_()
{
	OutputLock lock;
	if (wrefresh(M_WINDOW) == ERR)
		throw CursesError();
}

void CursesWindow::attron_(int attrs)
{
	OutputLock lock;
	if (wattron(M_WINDOW, attrs) == ERR)
		throw CursesError();
}

void CursesWindow::attroff_(int attrs)
{
	OutputLock lock;
	if (wattroff(M_WINDOW, attrs) == ERR)
		throw CursesError();
}


void CursesWindow::clear_()
{
	OutputLock lock;
	if (wclear(M_WINDOW) == ERR)
		throw CursesError();
}

int CursesWindow::curs_set_(int visibility)
{
	OutputLock lock;
	const int cursesReturn = curs_set(visibility);
	if (cursesReturn == ERR)
		throw CursesError();
	return cursesReturn;
}

void CursesWindow::cbreak_()
{
	OutputLock lock;
	if (cbreak() == ERR)
		throw CursesError();
}

void CursesWindow::nodelay_(bool bf)
{
	OutputLock lock;
	if (nodelay(M_WINDOW, bf) == ERR)
		throw CursesError();
}

void CursesWindow::timeout_(int delay)
{
	OutputLock lock;
	wtimeout(M_WINDOW, delay);
}

void CursesWindow::keypad_(bool bf)
{
	OutputLock lock;
	if (keypad(M_WINDOW, bf) == ERR)
		throw CursesError();
}

void CursesWindow::noecho_()
{
	OutputLock lock;
	if (noecho() == ERR)
		throw CursesError();
}

bool CursesWindow::has_colors_()
{
	OutputLock lock;
	return has_colors();
}

void CursesWindow::start_color_()
{
	OutputLock lock;
	if (start_color() == ERR)
		throw CursesError();
}

void CursesWindow::init_pair_(short pair, short f, short b)
{
	OutputLock lock;
	if (init_pair(pair, f, b) == ERR)
		throw CursesError();
}



int CursesWindow::getch_()
{
	InputLock lock;
	const int ch = wgetch(M_WINDOW);
	for (int i = 0; i < NUM_KEYS; ++i) {
		if (ch == keyTable[i][0])
			return keyTable[i][1];
	}
	// We haven't found the key, so return the original code.
	return ch;
}

void CursesWindow::printw_(const char* fmt)
{
	OutputLock lock;
	if (wprintw(M_WINDOW, fmt) == ERR)
		throw CursesError();
}

void CursesWindow::mvprintw_(int y, int x, const char* fmt)
{
	OutputLock lock;
	if (mvwprintw(M_WINDOW, y, x, fmt) == ERR)
		throw CursesError();
}

void CursesWindow::printw_(const char* fmt, const char* s)
{
	OutputLock lock;
	if (wprintw(M_WINDOW, fmt, s) == ERR)
		throw CursesError();
}

void CursesWindow::mvprintw_(int y, int x, const char* fmt, unsigned int d)
{
	OutputLock lock;
	if (mvwprintw(M_WINDOW, y, x, fmt, d) == ERR)
		throw CursesError();
}

void CursesWindow::mvprintw_(int y, int x, const char* fmt, float f)
{
	OutputLock lock;
	if (mvwprintw(M_WINDOW, y, x, fmt, f) == ERR)
		throw CursesError();
}

int CursesWindow::COLOR_PAIR_(short pair)
{
	OutputLock lock;
	return COLOR_PAIR(pair);
}

//
// Lock methods.
//

pthread_mutex_t CursesWindow::OutputLock::m_outputLock; 
pthread_mutex_t CursesWindow::InputLock::m_inputLock; 

void CursesWindow::OutputLock::create()
{
	pthread_mutex_init(&m_outputLock,NULL);
}

void CursesWindow::OutputLock::destroy()
{
	pthread_mutex_destroy(&m_outputLock);
}

CursesWindow::OutputLock::OutputLock()
{
	pthread_mutex_lock(&m_outputLock);
}

CursesWindow::OutputLock::~OutputLock()
{
	pthread_mutex_unlock(&m_outputLock);
}

void CursesWindow::InputLock::create()
{
	pthread_mutex_init(&m_inputLock,NULL);
}

void CursesWindow::InputLock::destroy()
{
	pthread_mutex_destroy(&m_inputLock);
}

CursesWindow::InputLock::InputLock()
{
	pthread_mutex_lock(&m_inputLock);
}

CursesWindow::InputLock::~InputLock()
{
	pthread_mutex_unlock(&m_inputLock);
}

