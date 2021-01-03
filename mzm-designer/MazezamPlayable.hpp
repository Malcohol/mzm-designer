/* ***************************************************************************
 * MazezamPlayable.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMPLAYABLE_H_
#define MAZEZAMPLAYABLE_H_

#include <mzm/Mazezam/MazezamHistory.hpp>
#include "Mode.hpp"

class CursesWindow;

/*!
 * A class for playing Mazezam.
 */
class MazezamPlayable : public mzm::MazezamHistory, public Mode
{
public:
	/*!
	 * Constructor.
	 * \param m the underlying level.
	 */
	MazezamPlayable(const mzm::MazezamData& m);

	/*!
	 * Destructor.
	 */
	virtual ~MazezamPlayable();

	// Mode interface methods.
	virtual std::string getModeName();
	virtual ModeReturnValue modeLoop(CursesWindow& w, bool useColors, Mode* parent);
	virtual ModeReturnValue processKey(CursesWindow& w, int k, Mode* next = NULL);	
	virtual void drawHelpInfo(CursesWindow& w, int y, int x, Mode* next = NULL);
private:
	/*!
	 * Copy constructor. Private and unimplemented.
	 */
	MazezamPlayable(const MazezamPlayable& m);
	/*!
	 * Copy assignment. Private and unimplemented.
	 */
	MazezamPlayable& operator=(const MazezamPlayable& other);
};

#endif /*MAZEZAMPLAYABLE_H_*/
