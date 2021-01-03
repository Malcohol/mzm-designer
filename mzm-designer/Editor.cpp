/* ***************************************************************************
 * Editor.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include <cassert>

#include "Editor.hpp"
#include "MazezamCurses.hpp"

Editor::Editor (mzm::mzm_coord h, mzm::mzm_coord w, mzm::mzm_coord s, mzm::mzm_coord f) 
: m_level(h, w, s, f)
, m_selection(h, w)
, m_cursX(0)
, m_cursY(0)
{
}

Editor::Editor (const mzm::MazezamData& m)
: m_level(m)
, m_selection(m.getHeight(), m.getWidth())
, m_cursX(0)
, m_cursY(0)
{
}

Editor::~Editor()
{
}


void Editor::toggle()
{
	if (m_cursX == -1) {
		if (!m_level.getBlock(m_cursY,0)) {
			m_level.setStart(m_cursY);
			setChanged();
		}
	} else if (m_cursX == m_level.getWidth()) {
		m_level.setFinish(m_cursY);
		setChanged();
	} else {
		if (m_cursY != m_level.getStart() || m_cursX != 0) {
			m_level.setBlock(m_cursY, m_cursX, !m_level.getBlock(m_cursY, m_cursX));
			setChanged();
		}
	}
}

void Editor::block()
{
	if ((m_cursX >= 0) && (m_cursX < m_level.getWidth())) {
		if (m_cursY != m_level.getStart() || m_cursX != 0) {
			m_level.setBlock(m_cursY, m_cursX, true);
			setChanged();
		}
	}
	if (m_cursX < m_level.getWidth())
		++m_cursX;
}

void Editor::gap()
{
	if ((m_cursX >= 0) && (m_cursX < m_level.getWidth())) {
		if (m_cursY != m_level.getStart() || m_cursX != 0) {
			m_level.setBlock(m_cursY, m_cursX, false);
			setChanged();
		}
	}
	if (m_cursX < m_level.getWidth())
		++m_cursX;
}


void Editor::backspace()
{
	if (m_cursX >= 0)
		--m_cursX;
	del();
}

void Editor::del()
{
	if ((m_cursX >= 0) && (m_cursX < m_level.getWidth()))
		m_level.setBlock(m_cursY, m_cursX, false);
	setChanged();
}

void Editor::shiftWest()
{
	// Check we're not overwriting the start position.
	if (m_level.canShiftWest(m_cursY) && ((m_cursY != m_level.getStart() || !m_level.getBlock(m_cursY, 1)))) {
		m_level.shiftWest(m_cursY);
		setChanged();
	}
}

void Editor::shiftEast()
{
	if (m_level.canShiftEast(m_cursY)) {
		m_level.shiftEast(m_cursY);	
		setChanged();
	}
}

bool Editor::setDimensions(mzm::mzm_coord h, mzm::mzm_coord w)
{
	if ((h == m_level.getHeight()) && (w == m_level.getWidth()))
		return true;
		
	if (!m_level.canSetDimensions(h, w))
		return false;

	m_level.setDimensions(h, w);
	m_selection.resize(h, w);
	// Ensure the cursor stays within bounds.
	m_cursX = std::min(m_cursX, m_level.getWidth());
	m_cursY = std::min(m_cursY, (mzm::mzm_coord) (m_level.getHeight() - 1));
	setChanged();
	return true;
}

Mode::ModeReturnValue Editor::processKey(CursesWindow& w, int k, Mode* next)
{
	// Get the dimensions of the window.
	int maxy, maxx;
	w.getmaxyx_(maxy,maxx);
	
	switch (k) {
		// NAVIGATION
		case CursesWindow::KEY_UP_:
			if (m_cursY > 0)
				--m_cursY;
			break;
		case CursesWindow::KEY_DOWN_:
			if (m_cursY < m_level.getHeight() - 1)
				++m_cursY;
			break;
		case CursesWindow::KEY_LEFT_:
			if (m_cursX > -1)
				--m_cursX;
			break;
		case CursesWindow::KEY_RIGHT_:
			if (m_cursX < m_level.getWidth())
				++m_cursX;
			break;
		case CursesWindow::KEY_HOME_:
			m_cursX = 0;
			break;
		case CursesWindow::KEY_END_:
			m_cursX = m_level.getWidth();
			break;
		// EDITING
		case CursesWindow::KEY_DC_:
			del();
			break;
		case CursesWindow::KEY_BACKSPACE_:
			backspace();
			break;
		case ' ':
			toggle();
			break;
		case 'b':
			block();
			break;
		case 'g':
			gap();
			break;
		case '<':
			shiftWest();
			break;
		case '>':
			shiftEast();
			break;
		case ']':
			if (m_level.getWidth() < std::min(maxx - 2, mzm::MAX_MAZEZAM_WIDTH)) {
				const mzm::mzm_coord newHeight = m_level.getHeight();
				const mzm::mzm_coord newWidth = m_level.getWidth() + 1;  
				setDimensions(newHeight, newWidth);
				w.clear_();
			}
			break;
		case '[':
			if (m_level.getWidth() > mzm::MIN_MAZEZAM_WIDTH) {
				const mzm::mzm_coord newHeight = m_level.getHeight();
				const mzm::mzm_coord newWidth = m_level.getWidth() - 1;  
				setDimensions(newHeight, newWidth);
				w.clear_();
			}
			break;
		case '}':
			if (m_level.getHeight() < std::min(maxy - 2, mzm::MAX_MAZEZAM_HEIGHT)) {
				const mzm::mzm_coord newHeight = m_level.getHeight() + 1;
				const mzm::mzm_coord newWidth = m_level.getWidth();  
				setDimensions(newHeight, newWidth);
				w.clear_();
			}
			break;
		case '{':
			if (m_level.getHeight() > mzm::MIN_MAZEZAM_HEIGHT) {
				const mzm::mzm_coord newHeight = m_level.getHeight() - 1;
				const mzm::mzm_coord newWidth = m_level.getWidth();  
				setDimensions(newHeight, newWidth);
				w.clear_();
			}
			break;
		case 'C':
			clear();
			setChanged();
			break;
		case 'R':
			randomize();
			setChanged();
			break;
		case 'm':
			if ((m_cursX >= 0) && (m_cursX <= m_level.getWidth() - 1))
				m_selection.select(m_cursY, m_cursX);
			//if (m_cursX < m_level.getWidth())
			//	++m_cursX;
			break;
		case 'M':
			//if (m_cursX > 0)
			//	--m_cursX;
			if ((m_cursX >= 0) && (m_cursX <= m_level.getWidth() - 1))
				m_selection.unselect(m_cursY, m_cursX);
			break;
		case 'a':
			m_selection.selectAll();
			break;
		case 'A':
			m_selection.clear();
			break;
		default:
			break;
	}
	if (next)
		return next->processKey(w, k);
	else
		return LOOP;	
}

Mode::ModeReturnValue Editor::modeLoop(CursesWindow& w, bool useColors, Mode* parent)
{
	// Make cursor visible.
  	CursesWindow::curs_set_(1);
  	// Make the window pick up extra keys.
	w.keypad_(true);
	// Make getch have blocking behaviour.
	w.nodelay_(false);
	Editor::ModeReturnValue ret = LOOP;
	do
	{
		MazezamCurses::draw(&w, useColors, *this);
		w.refresh_();
		notify();
		ret = parent->processKey(w, w.getch_(), this);
	} while (ret == LOOP);
	return ret;
}

std::string Editor::getModeName() {
	return "Editor";
}

struct KeyHelp 
{
	std::string key;
	std::string description;
};

KeyHelp editorHelpKeys[] = {
	{"b", "block"},
	{"g", "gap"},
	{"s", "save"},
	{"SPACE", "toggle"},
	{"DEL", "delete"},
	{"BS", "backspace"},
	{"</>", "shift l/r"},
	{"{/}", "height -/+"},
	{"[/]", "width -/+"},
	{"m(M)", "(un)select"},
	{"a(A)", "(un)selectAll"},
	{"C", "clear"},
	{"R", "randomize"}
};

int numEditorHelpKeys = sizeof(editorHelpKeys) / sizeof(KeyHelp);

void Editor::drawHelpInfo(CursesWindow& w, int y, int x, Mode* next)
{
	int maxx, maxy;
	w.getmaxyx_(maxy,maxx);
	const int numHelpKeys = std::min(numEditorHelpKeys, maxy-3); 
	for (int i = 0; i < numHelpKeys; ++i) {
		//wattron(w,A_UNDERLINE);
		w.mvprintw_(y + i,x,editorHelpKeys[i].key.c_str());
		//wattroff(w,A_UNDERLINE);
		w.printw_(" ");
		w.printw_(editorHelpKeys[i].description.c_str());
	}
}


void Editor::setLevel(const mzm::MazezamData& m)
{
	m_level = m;
	m_cursX = std::min(m_cursX, m_level.getWidth());
	m_cursY = std::min(m_cursY, (mzm::mzm_coord) (m_level.getHeight() - 1));
	//setChanged();
}

void Editor::randomize()
{
	const bool test = m_selection.hasSelectedPosition();
	for (int i = 0; i < m_level.getHeight(); ++i)
		for (int j = (i == m_level.getStart()); j < m_level.getWidth(); ++j)
			if (!test || m_selection.isSelected(i, j))
				m_level.setBlock(i, j, rand() % 2);
}

void Editor::clear()
{
	const bool test = m_selection.hasSelectedPosition();
	for (int i = 0; i < m_level.getHeight(); ++i)
		for (int j = (i == m_level.getStart()); j < m_level.getWidth(); ++j)
			if (!test || m_selection.isSelected(i, j))
				m_level.setBlock(i, j, false);
}

