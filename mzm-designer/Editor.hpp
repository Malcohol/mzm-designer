/* ***************************************************************************
 * Editor.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef EDITOR_H_
#define EDITOR_H_

#include <valarray>

#include <mzm/Mazezam/MazezamData.hpp>
#include <mzm/Mazezam/MazezamPerson.hpp>
#include <mzmcommon/Observer/Observer.hpp>
#include <mzm/MazezamSolver/Selection.hpp>
#include "CursesWindow.hpp"
#include "Mode.hpp"

/*!
 * Editor objects manage the interactive editing of a level.
 * They update their observer when their level has changed.
 */
class Editor : public Subject, public Mode
{
public:
	/*!
	 * The state of a new unedited Mazezam.
	 */
	enum defaultSizes {
		DEFAULT_MAZEZAM_HEIGHT = 4,
		DEFAULT_MAZEZAM_WIDTH = 5,
		DEFAULT_MAZEZAM_START = (DEFAULT_MAZEZAM_HEIGHT / 2),
		DEFAULT_MAZEZAM_FINISH = ((DEFAULT_MAZEZAM_HEIGHT - 1) / 2)
	};
public:
	/*!
	 * Construct an editor.
	 * \param h the initial height.
	 * \param w the initial width.
	 * \param s the initial start row.
	 * \param f the initial finish row.
	 */	
	Editor (mzm::mzm_coord h = DEFAULT_MAZEZAM_HEIGHT, mzm::mzm_coord w = DEFAULT_MAZEZAM_WIDTH, mzm::mzm_coord s = DEFAULT_MAZEZAM_START, mzm::mzm_coord f = DEFAULT_MAZEZAM_FINISH);
	/*!
	 * Constructor which takes a MazezamData.
	 */
	Editor (const mzm::MazezamData& m);
	/*!
	 * Destructor.
	 */
	virtual ~Editor();
	/*!
	 * Get the level currently in the editor.
	 */
	inline const mzm::MazezamData& getLevel() const;
	/*!
	 * Set the mazezam.
	 */
	virtual void setLevel( const mzm::MazezamData& m ); 
	/*!
	 * Fill the selected positions of the mazezam with random blocks/gaps.
	 */
	void randomize();
	/*!
	 * Clear the selected positions of the mazezam.
	 */
	void clear();
	/*!
	 * Get the x coord of the cursor.
	 */
	inline mzm::mzm_coord getCursX() const;
	/*!
	 * Get the y coord of the cursor.
	 */
	inline mzm::mzm_coord getCursY() const;
	/*!
	 * Return the selected positions.
	 */
	inline const mzm::Selection& getSelection() const;
	
	// Mode interface methods.
	virtual std::string getModeName();
	virtual ModeReturnValue modeLoop(CursesWindow& w, bool useColors, Mode* parent);
	virtual ModeReturnValue processKey(CursesWindow& w, int k, Mode* next = NULL);
	virtual void drawHelpInfo(CursesWindow& w, int y, int x, Mode* next);
private:
	/*!
	 * The Mazezam being edited.
	 */
	mzm::MazezamData m_level;
	/*!
	 * The set of selected positions.
	 */
	mzm::Selection m_selection;
	/*!
	 * The x coordinate of the cursor.
	 */
	mzm::mzm_coord m_cursX;
	/*!
	 * The y coordinate of the cursor.
	 */
	mzm::mzm_coord m_cursY;

private:
	/*!
	 * Toggle the state of the block at the cursor position, or adjust the
	 * start and finish position.
	 */
	void toggle();
	/*!
	 * Pub a block at the cursor position and move the cursor right.
	 */
	void block();
	/*!
	 * Pub a gap at the cursor position and move the cursor right.
	 */
	void gap();
	/*!
	 * Move the cursor left and erase a block.
	 */
	void backspace();
	/*!
	 * Delete a block at the cursor position.
	 */
	void del();
	/*!
	 * Shift the row at the cursor position west.
	 */
	void shiftWest();
	/*!
	 * Shift the row at the cursor position east.
	 */
	void shiftEast();
	/*!
	 * Change the height and width of the level.
	 */
	bool setDimensions(mzm::mzm_coord h, mzm::mzm_coord w);
private:
	/*!
	 * Copy constructor private and unimplemented.
	 */
	Editor (const Editor& other);
	/*!
	 * Copy assignement private and unimplemented.
	 */
	Editor& operator= (const Editor& other);
	
};

#include "Editor.inl"

#endif /*EDITOR_H_*/
