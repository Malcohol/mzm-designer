/* ***************************************************************************
 * Observer.cpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#include "Observer.hpp"

Subject::Subject() : m_observer(0), m_changed(false) { }

Subject::~Subject() { }

void Subject::setObserver(Observer* o)
{
	m_observer = o;
}

void Subject::setChanged()
{
	m_changed = true;
}

void Subject::notify()
{
	if (m_observer && m_changed)
		m_observer->update(this);
	m_changed = false;
}
