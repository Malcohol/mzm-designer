/* ***************************************************************************
 * Config.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef CONFIG_H_
#define CONFIG_H_

#include <mzm/Mazezam/MazezamData.hpp>

#include <cstdlib>

namespace mzm {

/*!
 * A Solver configuration representing a MazezaM level used for finding
 * solutions with the fewest moves.
 */
class Config
{
public:
	/*!
	 * Constructor.
	 * \param m the underlying level.
	 */
	Config(const MazezamData& m);
	/*!
	 * Copy constructor.
	 */
	Config(const Config& m);	
	/*!
	 * Assignment private and unimplemented.
	  */
	Config& operator=(const Config& c);
	/*!
	 * Destructor.
	 */
	~Config();
	
	/*!
	 * Can the player move North?
	 * \return true if the player can move North.
	 */
	inline bool canMoveNorth() const {
		return (m_y > 0) && !(row(m_y-1) & (1 << m_xx));
	}
	
	/*!
	 * Can the player move South?
	 * \return true if the player can move South.
	 */
	inline bool canMoveSouth() const {
		return (m_y < m_level.getHeight() - 1) && !(row(m_y+1) & (1 << m_xx));
	}

	/*!
	 * Can the player move East?
	 * \return true if the player can move East.
	 */
	inline bool canMoveEast() const {
		return (m_xx > 0) && !(row(m_y) & (1 << (m_xx - 1)));
	}
	
	/*!
	 * Can the player move West?
	 * \return true if the player can move West.
	 */
	inline bool canMoveWest() const {
		return (m_xx < m_level.getWidth() - 1) && !(row(m_y) & (1 << (m_xx + 1)));
	}
	
	/*!
	 * Can the player push East?
	 * \return true if the player can move East.
	 */
	inline bool canPushEast() const {
		return (m_xx > 0) && !(row(m_y) & 1) && (row(m_y) & (1 << (m_xx - 1)));
	}
	
	/*!
	 * Can the player push West?
	 * \return true if the player can push West.
	 */
	inline bool canPushWest() const {
		return (m_xx < m_level.getWidth() - 1) && (m_inset[m_y] > 0) && (row(m_y) & (1 << (m_xx + 1)));
	}

	/*!
	 * Returns true if the player is at the goal. 
	 */
	inline bool isGoal() const {
		return (m_xx == 0) && (m_y == m_level.getFinish()); 
	};

	/*!
	 * The Mazezams which can be reached by a single move.
	 * \param v the vector into which to place the configurations.
	 */
	void getNeighbours(std::vector<Config*>& v);
	
	/*!
	 * Returns an estimate of the distance to the goal.
	 */
	inline float getEstimatedDistance () const {
		return m_xx + ((mzm_coord) abs(((int) m_level.getFinish()) - ((int) m_y)));
	}

	/*!
	 * Compare by contents.
	 * \param other the other Mazezam.
	 */
	bool operator< (const Config& other) const;

	/*!
	 * Exception class thrown by addMoves if the other configuration cannot be reached.
	 */
	class NoSuchMoves {};
	
	/*!
	 * Add the necessary moves to get from a starting state to this one.
	 * \throw NoSuchMovess if the other configuration cannot be reached.
	 */
	void addInitialMoves(mzm_path& p, mzm_coord& x, mzm_coord& y) const;
	
	/*!
	 * Add the necessary moves to get from this Mazezam to the other.
	 * \throw NoSuchMoves if the other configuration cannot be reached.
	 */
	void addMoves(Config& other, mzm_path& p, mzm_coord& x, mzm_coord& y) const; 

	/*!
	 * Add the necessary moves to get from the last configuration to a winning state.
	 * \throw NoSuchMoves if the winning state cannot be reached.
	 */
	void addFinalMoves(mzm_path& p, mzm_coord& x, mzm_coord& y) const; 

	/*!
	 * Write a textual version of a configuration to an output stream.
	 */
	friend std::ostream& operator<< (std::ostream& os, Config& m);
	
public:
	/*!
	 * The underlying level.
	 */
	const MazezamData& m_level;
protected:	
	/*!
	 * The amount each row has been pushed rightwards.
	 */
	mzm_coord* m_inset;
	
	/*!
	 * The character's x coord counted from the right of the level.
	 */
	mzm_coord m_xx;
	
	/*!
	 * The character's y coord.
	 */
	mzm_coord m_y;
protected:
	/*!
	 * The state of the yth row.
	 * \param y the row.
	 * \return the state of the yth row.
	 */
	inline mzm_row row(mzm_coord y) const {
		return m_level.getRow(y) >> m_inset[y];
	}
private:
	/*!
	 * Private constructor.
	 */
	Config(const MazezamData& m, const mzm_coord* inset, mzm_coord xx, mzm_coord y);
	/*!
	 * Private constructor.
	 */
	Config(const MazezamData& m, const mzm_coord* inset, mzm_coord xx, mzm_coord y, bool wasLeft);
};

} // namespace mzm

#endif /*CONFIG_H_*/
