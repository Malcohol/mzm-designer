/* ***************************************************************************
 * ConfigWalk.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef CONFIGWALK_H_
#define CONFIGWALK_H_

#include <mzm/Mazezam/MazezamData.hpp>
#include <cstdlib>

namespace mzm {

/*!
 * A Solver configuration representing a MazezaM level used to find paths
 * from one player position to another without pushing rows. 
 */
class ConfigWalk
{
public:
	/*!
	 * Constructor.
	 * \param l the underlying level.
	 * \param x the players current x position.
	 * \param y the players current y position.
	 * \param tx the target x position.
	 * \param ty the target y position.
	 */
	ConfigWalk(const MazezamData& l, mzm_coord x, mzm_coord y, mzm_coord tx, mzm_coord ty);

	/*!
	 * Destructor.
	 */
	~ConfigWalk() {};
	
	/*!
	 * Returns the configurations which can be reached by a single push from the given
	 * configuration.
	 * \param v the vector of configurations to add to.
	 */
	void getNeighbours(std::vector<ConfigWalk*>& v) const;
	
	/*!
	 * Returns an estimate of the distance to the goal.
	 * \return an estimate of the distance to the goal.
	 */
	inline float getEstimatedDistance () 
	{
		return abs(((int) m_tx) - ((int) m_x)) + abs(((int) m_ty) - ((int) m_y));
	}
	
	/*!
	 * Return true if the player has reached the target position.
	 * \return true if the player has reached the target position.
	 */
	inline bool isGoal() const
	{ 
		return (m_x == m_tx) && (m_y == m_ty);
	}

	/*!
	 * Compare by contents.
	 * \param other the other Configuration.
	 * \return true if this is less than the other configuration.
	 */
	bool operator< (const ConfigWalk& other) const;
     
	/*!
	 * Exception class thrown by addMoves if the other configuration cannot be reached.
	 */
	class NoSuchMoves {};
	
	/*!
	 * Add the necessary moves to get from this configuration to other.
	 * \throw NoSuchMoves if the other configuration cannot be reached.
	 * \param other the configuration to reach.
	 * \param p the path to which moves are added.
	 * \param x the x coord at which to start, and the x coord which was reached.
	 * \param y the y coord at which to start, and the x coord which was reached.
	 */
	void addMoves(ConfigWalk& other, mzm_path& p, mzm_coord& x, mzm_coord& y);

	/*!
	 * The level this config corresponds to.
	 */
	const MazezamData& m_level;
protected:
	/*!
	 * The current x position.
	 */
	mzm_coord m_x;
	
	/*!
	 * The current y position.
	 */
	mzm_coord m_y;
	
	/*!
	 * The target x position.
	 */
	const mzm_coord m_tx;
	
	/*!
	 * The target y position.
	 */
	const mzm_coord m_ty;

protected:
	/*!
	 * The state of the yth row.
	 * \param y the row.
	 * \return the state of the yth row.
	 */
	inline mzm_row row(mzm_coord y) const {
		return m_level.getRow(y) >> m_level.getInset(y);
	}

private:
	/*!
	 * Assignment private and unimplemented.
	 */
	ConfigWalk& operator=(const ConfigWalk& c);
};

} // namespace mzm

#endif /*CONFIGWALK_H_*/
