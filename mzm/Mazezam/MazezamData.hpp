/* ***************************************************************************
 * MazezamData.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMDATA_H_
#define MAZEZAMDATA_H_

#include <cassert>
#include <mzm/Mazezam/TypesAndConstants.hpp>

namespace mzm {

/*!
 * This class contains the raw contents of a mazezam level.
 */
class MazezamData 
{
public:
	/*!
	 * Constructor.
	 * \param h the initial height.
	 * \param w the initial width.
	 * \param s the initial start row.
	 * \param f the initial finish row.
	 */	
	MazezamData (mzm_coord h, mzm_coord w, mzm_coord s, mzm_coord f);
	/*!
     * Constructor.
     * \param other the other Mazezam.
     * \param i new insets.
     */
	MazezamData (const MazezamData& other, const mzm_coord* i);
	
	/*!
	 * An array of Booleans, representing a row of blocks in a mazezam.
	 */
	typedef std::vector<bool> row_type;
	/*!
	 * An array or rows, representing the rows in a mazezam.
	 */
	typedef std::vector<row_type> array_type;
	 
	/*!
	 * Construct a Mazezam from an array of arrays of bools.
	 */
	MazezamData (array_type& block, mzm_coord h, mzm_coord w, mzm_coord s, mzm_coord f); 
	/*!
	 * Copy constructor.
	 */
	MazezamData (const MazezamData& other);
	/*!
	 * Copy assignment.
	 */
	MazezamData& operator= (const MazezamData& other);
	/*!
	 * Destructor.
	 */
	virtual ~MazezamData();
	
	// Accessors.
	
	/*!
	 * Provides const access to the block data.
	 */
	inline bool getBlock(mzm_coord y, mzm_coord x) const;
	/*!
	 * Can the row be shifted westward?
	 * \param r the row.
	 */
	inline bool canShiftWest(mzm_coord r) const;
	/*!
	 * Can the row be shifted eastward?
	 * \param r the row.
	 */
	inline bool canShiftEast(mzm_coord r) const;
	/*!
	 * Get the row data for row r.
	 */
	inline mzm_row getRow(mzm_coord r) const;
	/*!
	 * Get the inset data for row r.
	 */
	inline mzm_coord getInset(mzm_coord r) const;
	/*!
	 * Return the height.
	 */
	inline mzm_coord getHeight() const;
	/*!
	 * Return the width.
	 */
	inline mzm_coord getWidth() const;
	/*!
	 * Return the start row.
	 */
	inline mzm_coord getStart() const;
	/*!
	 * Return the finish row.
	 */
	inline mzm_coord getFinish() const;
	/*!
	 * Can the dimensions be set to the provided height and width?
	 * \param h the height.
	 * \param w the width.
	 */
	bool canSetDimensions(mzm_coord h, mzm_coord w) const;
	
	// Mutators.

	/*!
	 * Modify the block data.
	 */
	void setBlock(mzm_coord y, mzm_coord x, bool b);
	/*!
	 * Shift the row West.
	 * \param r the row.
	 */
	void shiftWest(mzm_coord r);
	/*!
	 * Shift the row East.
	 * \param r the row.
	 */
	void shiftEast(mzm_coord r);
	/*!
	 * Set the start.
	 */
	void setStart(mzm_coord y);
	/*!
	 * Set the finish.
	 */
	void setFinish(mzm_coord y);
	/*!
	 * Change the height and width of the level.
	 * \param h the height.
	 * \param w the width.
	 */
	void setDimensions(mzm_coord h, mzm_coord w);
	/*!
	 * Empties the mazezam of blocks.
	 */
	void clear();
private:
    /*! 
     * The row data in binary form, "left aligned".
     * If a row is non-zero, then its (m_width - 1)th least significant
     * bit must be 1 and any higher bits must be zero.
     */
    mzm_row* m_row;
    /*! 
     * The insets of the rows.
     * An empty row has an inset equal to the width.
     */
    mzm_coord* m_inset;
	/*!
	 * The height of the level.
	 */
	mzm_coord m_height;
	/*!
	 * The width of the level.
	 */
	mzm_coord m_width;
	/*!
	 * The start row.
	 */
	mzm_coord m_start;
	/*!
	 * The finish row.
	 */
	mzm_coord m_finish;
public:
	/*!	
	 * Asserts the validity of the mzm_t.
	 * \return true.
	 */
	virtual bool isValid() const;
	/*!
	 * Assert that the provided insets are consistent with the row data.
	 * \return true. 
	 */
	bool checkInsets(const mzm_coord* inset) const;
	/*!
	 * Assert that the level is in a valid start position.
	 * \return true.
	 */
	bool checkStartPosition() const;
};

/*!
 * Write a level to an ostream.
 */
std::ostream& operator<< (std::ostream& os, const MazezamData& l);

} // namespace mzm

#include "MazezamData.inl"

#endif /*MAZEZAMDATA_H_*/
