/* ***************************************************************************
 * MazezamPerson.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMPERSON_H_
#define MAZEZAMPERSON_H_

#include <mzm/Mazezam/MazezamData.hpp>

namespace mzm {

/*!
 * A Mazezam with a person in it.
 */
class MazezamPerson : public MazezamData
{
public:
	/*!
	 * Constructor.
	 */
	MazezamPerson (mzm_coord h, mzm_coord w, mzm_coord s, mzm_coord f);
	/*!
	 * Copy constructor.
	 */
	MazezamPerson (const MazezamPerson& other);
	/*!
	 * Assignment.
	 */
	MazezamPerson& operator= (const MazezamPerson& other);
	/*!
	 * Constructor from a mzm_t.
	 */
	MazezamPerson (const MazezamData& m);
	
	// Accessors.
	
	/*!
	 * Return the person's x coord.
	 */
	inline mzm_coord getX() const;
	/*!
	 * Return the person's y coord.
	 */
	inline mzm_coord getY() const;
	/*!
	 * Can the player move North?
	 * \return true if the player can move North.
	 */
	inline bool canMoveNorth() const;
	/*!
	 * Can the player move South?
	 * \return true if the player can move South.
	 */
	inline bool canMoveSouth() const;
	/*!
	 * Can the player move East?
	 * \return true if the player can move East.
	 */
	inline bool canMoveEast() const;
	/*!
	 * Can the player move West?
	 * \return true if the player can move West.
	 */
	inline bool canMoveWest() const;
	/*!
	 * Can the player push East?
	 * \return true if the player can move East.
	 */
	inline bool canPushEast() const;
	/*!
	 * Can the player push West?
	 * \return true if the player can push West.
	 */
	inline bool canPushWest() const;
	/*!
	 * Is the person in the entrance? 
	 */
	inline bool atEntrance() const;
	/*!
	 * Is the person in the exit?
	 */
	inline bool atExit() const;
	
	// Mutators.
	
	/*!
	 * Move the player north.
	 */
	virtual void moveNorth();
	/*!
	 * Move the player south.
	 */
	virtual void moveSouth();
	/*!
	 * Move the player east.
	 */
	virtual void moveEast();
	/*!
	 * Move the player west.
	 */
	virtual void moveWest();
	/*!
	 * Push eastwards.
	 */
	virtual void pushEast();
	/*!
	 * Push westwards.
	 */
	virtual void pushWest();
	/*!
	 * Make the person move in the supplied direction.
	 */
	void moveInDirection(mzm_direction d);
	/*!
	 * Try to find a path from the person's present position to the specified position
	 * which involves no pushes.
	 * \return true if there was a path.
	 */
	bool findPath(mzm_coord y, mzm_coord x, mzm_path& p) const;
	/*!
	 * Apply the directions in a path.
	 * \param numSteps follow this many steps, or all the way if numSteps is negative.
	 */
	void followPath(const mzm_path& p, int numSteps = -1);
private:
	/*!
	 * The person's y coordinate.
	 */
	mzm_coord m_y;
	/*!
	 * The person's x coordinate.
	 */
	mzm_coord m_x;
};

} // namespace mzm

#include "MazezamPerson.inl"

#endif /*MAZEZAMPERSON_H_*/
