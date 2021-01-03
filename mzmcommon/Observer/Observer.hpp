/* ***************************************************************************
 * Observer.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef OBSERVER_H_
#define OBSERVER_H_

class Subject;

/*!
 * Observers should publicly inherit from this.
 */
class Observer
{
public:
	virtual void update(Subject* subject) = 0;
};

/*!
 * For simplicity, my subjects support a single observer.
 */
class Subject
{
public:
	/*!
	 * Constructor.
	 */
	Subject();
	/*!
	 * Destructor.
	 */
	virtual ~Subject();
	/*!
	 * Set the Observer object. 
	 */
	void setObserver(Observer* o);
protected:
	/*!
	 * Call this when the object has been changed.
	 */
	void setChanged();
	/*!
	 * Notifies the observer, if the object has been changed.
	 */
	void notify();
private:
	/*!
	 * The Observer object observing this object.
	 */
	Observer* m_observer;
	/*!
	 * True if the object has been changed.
	 */
	bool m_changed;
};

#endif /*OBSERVER_H_*/
