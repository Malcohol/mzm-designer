/* ***************************************************************************
 * CursesWindow.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef CURSESWINDOW_H_
#define CURSESWINDOW_H_

#include <pthread.h>

/*!
 * An object-oriented interface to a curses window, and serializes all use of the curses
 * library. Moreover, it avoids use of macros.
 * It is a very basic implementation which supports only a subset of curses useful to me.
 */
class CursesWindow 
{
public:
	/*!
	 * Exception thrown when there is a curses error.
	 */
	class CursesError {};
	/*!
	 * Get the outer window. The caller must delete the pointer, which will close curses.
	 */
	static CursesWindow* getOuterWindow();
	/*!
	 * Create a subwindow.
	 */
	CursesWindow(CursesWindow* parentWindow, int height, int width, int y, int x);

	/*!
	 * Lock the window.
	 */
	void lock();
	
	/*!
	 * Unlock the window.
	 */
	void unlock();
	
	//
	// Wrappers for terminal level functions.
	//
	static int curs_set_(int visibility);
	static void cbreak_();
	static void noecho_();
	static bool has_colors_();
	static void start_color_();
	static void init_pair_(short pair, short f, short b);
	static int COLOR_PAIR_(short pair);
	
	//
	// Wrapper functions for window level functions.
	//
	void getmaxyx_(int& y, int& x) const;
	void getbegyx_(int& y, int& x) const;
	void box_(int verch, int horch);
	void addch_(int ch);
	void mvaddch_(int y, int x, int ch);
	void hline_(int ch, int n);
	void mvhline_(int y, int x, int ch, int n);
	void mvvline_(int y, int x, int ch, int n);
	void move_(int y, int x);
	void refresh_();
	void attron_(int attrs);
	void attroff_(int attrs);
	void clear_();
	void nodelay_(bool bf);
	void timeout_(int delay);
	void keypad_(bool bf);
	
	/*!
	 * Special return values from getch.
	 */
	enum CursesKeys {
		KEY_UP_ = 256, // Well outside the ASCII range.
		KEY_DOWN_,
		KEY_LEFT_,
		KEY_RIGHT_,
		KEY_HOME_,
		KEY_END_,
		KEY_DC_,
		KEY_BACKSPACE_,
		ERR_,
		NUM_KEYS
	};
	
	/*!
	 * Returns one of the CursesKeys values, or ERR_, or a char.
	 */ 
	int getch_();
	
	//
	// Constants.
	//
	static const int A_UNDERLINE_;
	static const int A_REVERSE_;
	static const int COLOR_BLACK_;
	static const int COLOR_RED_;
	static const int COLOR_GREEN_;
	static const int COLOR_YELLOW_;
	static const int COLOR_BLUE_;
	static const int COLOR_MAGENTA_;
	static const int COLOR_CYAN_;
	static const int COLOR_WHITE_;
	
	//
	// Vars
	//
	static int ACS_LTEE_;
	static int ACS_RTEE_;
	static int ACS_TTEE_;
	static int ACS_BTEE_;
	static int ACS_HLINE_;
	static int ACS_VLINE_;
	
	//
	// The following functions cannot be wrapped in a general way, so only one
	// extra argument is supported.
	//
	void printw_(const char* fmt);
	void printw_(const char* fmt, const char* s);
	void mvprintw_(int y, int x, const char* fmt);
	void mvprintw_(int y, int x, const char* fmt, unsigned int d);
	void mvprintw_(int y, int x, const char* fmt, float f);
		
	/*!
	 * Destructor.
	 */
	virtual ~CursesWindow();
private:
	/*!
	 * Private default constructor builds the outer window.
	 */
	CursesWindow();
	/*!
	 * The parent window, which is NULL if this is the outerWindow.
	 */
	CursesWindow *const m_parent;
	/*!
	 * pointer to the curses window.
	 */
	void* m_window;
	/*!
	 * A lock for this particular window.
	 */
	pthread_mutex_t m_windowLock;

	/*!
	 * A singleton for the outer window. 
	 */
	static CursesWindow* m_outerWindow;
	
	//
	// Locking (I'm assuming input and output can be interleaved here).
	//
	
	/*!
	 * Objects of this class lock output to curses while in scope.
	 */
	class OutputLock 
	{
	public:
		OutputLock();
		~OutputLock();
		static void create();
		static void destroy();
	private:
		/*!
		 * A mutex which serializes all use of the curses library for output.
		 */
		static pthread_mutex_t m_outputLock;
	};
	
	/*!
	 * Objects of this class lock input from curses while in scope.
	 */
	class InputLock 
	{
	public:
		InputLock();
		~InputLock();
		static void create();
		static void destroy();
	private:
		/*!
		 * A mutex which serializes all use of the curses library for input.
		 */
		static pthread_mutex_t m_inputLock;
	};	
};


#endif /*CURSESWINDOW_H_*/
