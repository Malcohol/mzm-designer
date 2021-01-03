/* ***************************************************************************
 * IntervalSet.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef INTERVALSET_H_
#define INTERVALSET_H_

#include<functional>

/*!
 * A set which manages its contents as a tree of closed intervals. 
 * Note: the tree is not balanced.
 * 
 * The parameter CompareBelow is by default defined to be CompareLess, but can
 * be defined differently to allow neighbouring intervals to merge. 
 * For example, if T=int we might have
 * less(4,5) = false
 * below(4,5) = true.
 * Then intervals [2,4] and [5,10] would be merged into a single interval [2,10].
 * 
 * \param T the value type being stored.
 * \param CompareLess a comparison class.
 * \param CompareBelow a comparison class which may compare neighbouring values differently to CompareLess.
 */
template<class T, class CompareLess = std::less<T>(), class CompareBelow = CompareLess>
class IntervalSet
{
	/*!
	 * The comparison object for less.
	 */
	CompareLess& m_less;
	/*!
	 * The comparison object for below.
	 */
	CompareBelow& m_below;
	
	/*!
	 * The nodes out of which IntervalSets are built. 
	 */	
	class IntervalNode
	{
		/*!
	 	 * The comparison object for less.
	 	 */
		CompareLess& m_less;
		/*!
	 	 * The comparison object for below.
	 	 */
		CompareBelow& m_below;
		/*!
		 * The lower value of this interval.
		 */
		T m_lower;
		/*!
		 * The upper value of this interval.
		 */
		T m_upper;
		/*!
		 * The left subtree, if it exists.
		 */
		IntervalNode* m_left;
		/*!
		 * The right subtree, if it exists.
		 */
		IntervalNode* m_right;
		/*!
		 * Extend the lower value of this node to n.
		 * \param n the value to extend this node to, and also a new value to extend it to.
		 * \return a pointer to a possible replacement for this node.
		 */
		IntervalNode* extendLower(T& n);
		/*!
		 * Extend the upper value of this node to n.
		 * \param n the value to extend this node to, and also a new value to extend it to.
		 * \return a pointer to a possible replacement for this node.
		 */
		IntervalNode* extendUpper(T& n);

	public:
		/*!
		 * Constructor.
		 * \param l the lower value.
		 * \param u the upper value.
		 * \param less the less comparison object.
		 * \param below the below comparison object.
		 */
		IntervalNode(T l, T u, CompareLess less, CompareBelow below);
		
		/*!
		 * Destruct the subtree at this node.
		 */
		~IntervalNode();
		
		/*!
		 * Let the values l thru u be added to the set.
		 * \param l the lower value of the interval.
		 * \param u the upper value of the interval.
		 */
		void insert(T l, T u);
		/*!
		 * Does the set contains n.
		 * \param n the value to check.
		 * \return true iff the value is in the set.
		 */
		bool contains(T n);
	};
	
	/*!
	 * The root node of the tree, if there is one.
	 */
	IntervalNode* m_root;

	/*!
	 * Copy constructor private and unimplemented.
	 */
	IntervalSet(const IntervalSet& i);
	/*!
	 * Assignment private and unimplemented.
	 */
	IntervalSet& operator= (const IntervalSet& i);
public:
	/*!
	 * Constructs an initially empty interval set.
	 */
	IntervalSet(CompareLess less = CompareLess(), CompareBelow below = CompareBelow());
	
	/*!
	 * Destructor.
	 */
	virtual ~IntervalSet();
	
	/*!
	 * Let the values l thru u be added to the set.
	 * \param l the lower value of the interval.
	 * \param u the upper value of the interval.
	 */
	void insert(T l, T u);
	
	/*!
	 * Does the set contains n.
	 * \param n the value to check.
	 * \return true iff the value is in the set.
	 */
	bool contains(T n) const;
};

/* ************************************************************************
 * TEMPLATE DEFINITIONS FOR INTERVAL SET
 * ************************************************************************/

template<class T, class CompareLess, class CompareBelow>
IntervalSet<T,CompareLess,CompareBelow>::IntervalSet(CompareLess less, CompareBelow below) :
	m_less(less), m_below(below), m_root(0)
{
}

template<class T, class CompareLess, class CompareBelow>
IntervalSet<T,CompareLess,CompareBelow>::~IntervalSet()
{
	if (m_root)
		delete m_root;
}

template<class T, class CompareLess, class CompareBelow>
void IntervalSet<T,CompareLess,CompareBelow>::insert(T l, T u)
{
	if (m_root)
		m_root->insert(l,u);
	else
		m_root = new IntervalNode(l,u,m_less,m_below);
}

template<class T, class CompareLess, class CompareBelow>
bool IntervalSet<T,CompareLess,CompareBelow>::contains(T n) const
{
	return m_root && m_root->contains(n);
}

/* ************************************************************************
 * TEMPLATE DEFINITIONS FOR INTERVAL NODE 
 * ************************************************************************/

template<class T, class CompareLess, class CompareBelow>
IntervalSet<T,CompareLess,CompareBelow>::IntervalNode::IntervalNode(T l, T u, CompareLess less, CompareBelow below) :
	m_less(less), m_below(below), m_lower(l), m_upper(u), m_left(0), m_right(0) 
{ }

template<class T, class CompareLess, class CompareBelow>
IntervalSet<T,CompareLess,CompareBelow>::IntervalNode::~IntervalNode()
{
	if (m_left)
		delete m_left;
	if (m_right)
		delete m_right;
}

template<class T, class CompareLess, class CompareBelow>
bool IntervalSet<T,CompareLess,CompareBelow>::IntervalNode::contains(T n)
{
	if (m_less(n,m_lower))
		return m_left && m_left->contains(n);
	else if (m_less(m_upper, n))
		return m_right && m_right->contains(n);
	else
		return true;
}

template<class T, class CompareLess, class CompareBelow>
void IntervalSet<T,CompareLess,CompareBelow>::IntervalNode::insert(T l, T u)
{
	if (m_below(u,m_lower)) {
		// The new interval is totally below this node
		if (m_left)
			m_left->insert(l,u);
		else
			m_left = new IntervalNode(l,u,m_less,m_below);
	} else if (m_below(m_upper,l)) {
		// The new interval is totally above this node
		if (m_right)
			m_right->insert(l,u);
		else
			m_right = new IntervalNode(l,u,m_less,m_below);
	} else {
		// Otherwise it must overlap this node.
		// We extend this node left and right, if necessary.
		if (m_less(l,m_lower)) {
			if (m_left)
				m_left = m_left->extendLower(l);
			m_lower = l;
		}
		if (m_less(m_upper,u)) {
			if (m_right)
				m_right = m_right->extendUpper(u);
			m_upper = u;
		}
	}
}

template<class T, class CompareLess, class CompareBelow>
typename IntervalSet<T,CompareLess,CompareBelow>::IntervalNode* IntervalSet<T,CompareLess,CompareBelow>::IntervalNode::extendLower(T& n)
{
	if (m_below(m_upper,n)) {
		// If this node is lower than the extention.
		if (m_right)
			m_right = m_right->extendLower(n);
		return this;
	} else if (m_less(n,m_lower)) {
		// If this node is entirely contained within the extension.
		// Delete this node and its right subtree. Keep extending down the left subtree.
		IntervalNode* temp = m_left;
		// remember not to delete the left subtree.
		m_left = 0;
		delete this;
		if (temp)
			return temp->extendLower(n);
		else
			return 0;
	} else {
		// This node must overlap with the extension.
		n = m_lower;
		// Delete this node and its right subtree. The left subtree can be returned.
		IntervalNode* temp = m_left;
		// remember not to delete the left subtree.
		m_left = 0;
		delete this;
		return temp;
	}
}

template<class T, class CompareLess, class CompareBelow>
typename IntervalSet<T,CompareLess,CompareBelow>::IntervalNode* IntervalSet<T,CompareLess,CompareBelow>::IntervalNode::extendUpper(T& n)
{
	if (m_below(n,m_lower)) {
		// If this node is above than the extention.
		if (m_left)
			m_left = m_left->extendUpper(n);
		return this;
	} else if (m_less(m_upper,n)) {
		// If this node is entirely contained within the extension.
		// Delete this node and its left subtree. Keep extending down the right subtree.
		IntervalNode* temp = m_right;
		// remember not to delete the right subtree.
		m_right = 0;
		delete this;
		if (temp)
			return temp->extendUpper(n);
		else
			return 0;
	} else {
		// This node must overlap with the extension.
		n = m_upper;
		// Delete this node and its left subtree. The right subtree can be returned.
		IntervalNode* temp = m_right;
		// remember not to delete the right subtree.
		m_right = 0;
		delete this;
		return temp;
	}
}

#endif /*INTERVALSET_H_*/
