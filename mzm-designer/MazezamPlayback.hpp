/* ***************************************************************************
 * MazezamPlayback.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMPLAYBACK_H_
#define MAZEZAMPLAYBACK_H_

#include <mzm/Mazezam/MazezamHistory.hpp>
#include "Mode.hpp"

class CursesWindow;

/*!
 * A class for watching Mazezam being played back.
 */
class MazezamPlayback : public mzm::MazezamHistory, public Mode
{
public:
	/*!
	 * Constructor.
	 * \param m the underlying level.
	 * \param p a path which solves the mazezam.
	 */
	MazezamPlayback(const mzm::MazezamData& m, const mzm::mzm_path& p);
	
	/*!
	 * Destructor.
	 */
	virtual ~MazezamPlayback();
	
	/*!
	 * Mode interface methods.
	 */
	virtual std::string getModeName();
	virtual ModeReturnValue modeLoop(CursesWindow& w, bool useColors, Mode* parent);
	virtual ModeReturnValue processKey(CursesWindow& w, int k, Mode* next = NULL);
	void drawHelpInfo(CursesWindow& w, int y, int x, Mode* next = NULL);
public:
	/*!
	 * Is playback paused.
	 */
	bool m_paused;
	/*!
	 * The delay between playback steps.
	 */
	int m_delay;
private:
	/*!
	 * Copy constructor. Private and unimplemented.
	 */
	MazezamPlayback(const MazezamPlayback& m);
	/*!
	 * Copy assignment. Private and unimplemented.
	 */
	MazezamPlayback& operator=(const MazezamPlayback& other);
};

#endif /*MAZEZAMPLAYBACK_H_*/
