/* ***************************************************************************
 * ConfigEqv.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef CONFIGEQV_H_
#define CONFIGEQV_H_

#include <mzm/Mazezam/MazezamData.hpp>

namespace mzm {

/*!
 * A Solver configuration representing a MazezaM level used to find solutions
 * with the fewest pushes.
 * It abstracts the actual position of the player into "zones" consisting of
 * all reachable position.
 * By significantly reducing the search space, this tends to offer faster
 * solutions than Mazezam.
 */
class ConfigEqv
{
public:
	/*!
	 * Constructor.
	 * \param m the underlying level.
	 */
	ConfigEqv(const MazezamData& m);
	
	/*!
	 * Destructor.
	 */
	~ConfigEqv();
		
	/*!
	 * Returns the configurations which can be reached by a single push from the given
	 * configuration.
	 * \param v the vector into which to place configurations.
	 */
	void getNeighbours(std::vector<ConfigEqv*>& v) const;
	
	/*!
	 * Returns an estimate of the distance to the goal.
	 * 
	 * This currently returns 0 or 1, depending on whether the configuration is
	 * the goal.
	 * FIXME find a better heuristic.
	 */
	float getEstimatedDistance () const;
	
	/*!
	 * Returns true if the rightmost position on the given row is contained
	 * in the zone of the given configuration.
	 * \returns true if the rightmost position on the given row is contained
	 * in the zone of the given configuration.
	 */
	inline bool isGoal() const { 
		return m_zone[m_level.getFinish()] & 1;
	}
	
	/*!
	 * Compare by contents.
	 * \param other the other Configuration.
	 */
	bool operator< (const ConfigEqv& other) const;
	
	/*!
	 * Exception class thrown by addMoves if the other configuration cannot be reached.
	 */
	class NoSuchMoves {};
	
	/*!
	 * Add the necessary moves to get from a starting state to this one.
	 * \throw NoSuchMoves if the other configuration cannot be reached.
	 * \param p the path to which moves are added.
	 * \param x the x coord which was reached.
	 * \param y the y coord which was reached.
	 */
	void addInitialMoves(mzm_path& p, mzm_coord& x, mzm_coord& y) const;
	
	/*!
	 * Add the necessary moves to get from this configuration to other.
	 * \throw NoSuchMoves if the other configuration cannot be reached.
	 * \param other the configuration to reach.
	 * \param p the path to which moves are added.
	 * \param x the x coord at which to start, and the x coord which was reached.
	 * \param y the y coord at which to start, and the y coord which was reached.
	 */
	void addMoves(ConfigEqv& other, mzm_path& p, mzm_coord& x, mzm_coord& y) const; 
	
	/*!
	 * Add the necessary moves to get from the last configuration to a winning state.
	 * \throw NoSuchMoves if the winning state cannot be reached.
	 * \param p the path to which moves are added.
	 * \param x the x coord at which to start, and the y coord which was reached.
	 * \param y the y coord at which to start, and the x coord which was reached.
	 */
	void addFinalMoves(mzm_path& p, mzm_coord& x, mzm_coord& y) const;
	
	/*!
	 * Write a textual version of a level to an output stream.
	 */
	friend std::ostream& operator<< (std::ostream& os, ConfigEqv& c);
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
	 * The positions currently reachable by the player.
	 */
	mzm_row* m_zone;
protected:
	/*!
	 * The state of the yth row.
	 * \param y the row.
	 * \return the state of the yth row.
	 */
	inline mzm_row row(mzm_coord y) const {
		return m_level.getRow(y) >> m_inset[y];
	}

	/*!
	 * Extend the zone with all nodes reachable from (x,y) and modify
	 * priority.
	 * \param xxx the players current x position, represented in the form (1 << x).
	 * \param y the row 
	 */
	void buildZone(int xxx, mzm_coord y);

	/*!
	 * Find the x and y coord at which a push in direction will reach other.
	 * \param other the other MazezamEqv.
	 * \param x a reference in which the x value is placed.
	 * \param y a reference in which the y value is placed.
	 * \param isLeft a reference in which the direction is placed.
	 */
	void findPush(ConfigEqv& other, mzm_coord& x, mzm_coord& y, bool& isLeft) const;
	
	/*!
	 * Assignment private and unimplemented.
	 */
	ConfigEqv& operator=(const ConfigEqv& m);
	
	/*!
	 * Copy constructor private and unimplemented;
	 */
	ConfigEqv(const ConfigEqv& m);
	
	/*!
	 * Private constructor.
	 * \param l the underlying level.
	 * \param inset the insets.
	 * \param xxx the players current x position, represented in the form (1 << x).
	 * \param y the players current y position.
	 */
	ConfigEqv(const MazezamData& l, const mzm_coord* inset, int xxx, mzm_coord y);

	/*!
	 * Private constructor constructs the configuration after a push.
	 * \param l the underlying level.
	 * \param inset the state of the insets prior to the push.
	 * \param xxx the players current x position, represented in the form (1 << x).
	 * \param y the players current y position.
	 * \param wasLeft if the push that got the player to (x,y) was left or right
	 */
	ConfigEqv(const MazezamData& l, const mzm_coord* inset, mzm_row xxx, mzm_coord y, bool wasLeft);
};

} // namespace mzm

#endif /*CONFIGEQV_H_*/
