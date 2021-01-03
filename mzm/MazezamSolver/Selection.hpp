/* ***************************************************************************
 * Selection.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef SELECTION_H_
#define SELECTION_H_

#include <mzm/Mazezam/TypesAndConstants.hpp>

namespace mzm {

/*!
 * Defines a set of currently selected coordinates.
 * Used by the improver to determine which positions are
 * modifiable.
 */
class Selection
{
public:
	/*!
	 * Constructor.
	 */
	Selection(mzm_coord height, mzm_coord width);
	/*!
	 * Destructor.
	 */
	virtual ~Selection();
	/*!
	 * Is the position given position selected? 
	 */
	bool isSelected(mzm_coord y, mzm_coord x) const;
	/*!
	 * Is at least one position selection?
	 */
	bool hasSelectedPosition() const;
	/*!
	 * Add the given position to the selection.
	 */	
	void select(mzm_coord y, mzm_coord x);
	/*!
	 * Remove the given position from the selection.
	 */
	void unselect(mzm_coord y, mzm_coord x);
	/*!
	 * Clear the selection.
	 */
	void clear();
	/*!
	 * Selected all positions.
	 */
	void selectAll();
	/*!
	 * Selected a rectangle of positions.
	 */
	void selectRectangle(mzm_coord y, mzm_coord x, mzm_coord h, mzm_coord w);
	/*!
	 * Resize the selectable area.
	 */
	void resize(mzm_coord newHeight, mzm_coord newWidth);
private:
	/*!
	 * The row data.
	 */
	mzm_row* m_row;
	/*!
	 * The height of the selection area.
	 */
	mzm_coord m_height;
	/*!
	 * The width of the selection area.
	 */
	mzm_coord m_width;
};

} // namespace mzm

#endif /*SELECTION_H_*/
