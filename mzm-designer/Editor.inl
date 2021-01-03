/* ***************************************************************************
 * Editor.inl
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

mzm::mzm_coord Editor::getCursX() const 
{
	return m_cursX;
}

mzm::mzm_coord Editor::getCursY() const 
{
	return m_cursY;
}

const mzm::MazezamData& Editor::getLevel() const
{
	return m_level;
}

const mzm::Selection& Editor::getSelection() const
{
	return m_selection;
}
