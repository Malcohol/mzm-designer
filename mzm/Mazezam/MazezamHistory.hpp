/* ***************************************************************************
 * MazezamHistory.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMHISTORY_H_
#define MAZEZAMHISTORY_H_

#include <mzm/Mazezam/MazezamPerson.hpp>

namespace mzm {

/*!
 * A MazezamPerson that preserves the person's journey and supports resetting, undoing and redoing.
 */
class MazezamHistory : public MazezamPerson
{
public:
	/*!
	 * Constructor from a mzm_t.
	 */
	MazezamHistory (const MazezamData& m);
	/*!
	 * Constructor from a mzm_t with a pre-existing path.
	 * The player will follow the path to the index, where
	 * -1 indicates the player should follow the path all the way.
	 */
	MazezamHistory(const MazezamData& m, const mzm_path& p, int index = -1);
	
	/*!
	 * Restore the level to its original state.
	 */
	virtual void reset();
	/*!
	 * Is there a move that can be undone?
	 */
	virtual bool canUndo();
	/*!
	 * Undo the last move.
	 */
	virtual void undo();
	/*!
	 * Is there a move that can be redone?
	 */
	virtual bool canRedo();
	/*!
	 * Redo the last undone move.
	 */
	virtual void redo();
	
	// MazezamPerson methods.
	virtual void moveNorth();
	virtual void moveSouth();
	virtual void moveEast();
	virtual void moveWest();
	virtual void pushEast();
	virtual void pushWest();
protected:
	/*!
	 * Pull eastwards, used when undoing a pushWest.
	 * Does not modify m_path.
	 */
	virtual void pullEast();	
	/*!
	 * Pull westwards, used when undoing a pushEast.
	 * Does not modify m_path.
	 */
	virtual void pullWest();
private:
	/*!
	 * A path of moves from the start.
	 */
	mzm_path m_path;
	/*!
	 * The point in the path the character is currently at.
	 */
	mzm_path::iterator m_iterator;
};

} // namespace mzm

#endif /*MAZEZAMHISTORY_H_*/
