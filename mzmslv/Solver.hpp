/* ***************************************************************************
 * Solver.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef SOLVER_H_
#define SOLVER_H_

#include<vector>
#include<queue>
#include<algorithm>
#include<map>
#include<stack>
#include<set>
#include<cassert>

#include "SolverTypes.hpp"

namespace mzmslv {

/*!
 * The solver class.
 * \param C a configuration.
 */
template<class C>
class Solver
{
public:
	/*!
	 * Paths are sequences of Configurations.
	 */
	typedef std::vector<C*> path;

	/*!
	 * Constructor.
	 */	
	Solver() : m_keepSolving(true) { }
	
	/*!
	 * Destructor.
	 */
	virtual ~Solver() { }

	/*!
	 * Use a solver of the appropriate type to solve the problem.
	 */
	SolverResult findSolution(SearchType type, C* init, path& p);

	/*!
	 * Find a best solution if there is one using a breadth-first search.
	 * For consistency of memory management, init will be put on p regardless.
	 * \param init the initial configuration.
	 * \param p the path to add the solution to.
	 */
	SolverResult findSolutionBreadthFirst(C* init, path& p);
	
	/*!
	 * Finds a solution if there is using a depth first search.
	 * For consistency of memory management, init will be put on p regardless.
	 * \param init the initial configuration.
	 * \param p the path to add the solution to.
	 */
	SolverResult findSolutionDepthFirst(C* init, path& p);

	/*!
	 * Finds a solution if there is one using a greedy best-first search. 
	 * It's not guaranteed to find the shortest solution. Uses getEstimatedDistance.
	 * For consistency of memory management, init will be put on p regardless.
	 * \param init the initial configuration.
	 * \param p the path to add the solution to.
	 */
	SolverResult findSolutionBestFirst(C* init, path& p);
	
	/*!
	 * Finds an solution if there is one using A*, assuming that the
	 * getEstimatedDistance() heuristic of parameter C is consistent.
	 * getEstimatedDistance is called only once per configuration,
	 * so it need not be very efficient.
	 * \param init the initial configuration.
	 * \param p the path to add the solution to.
	 */
	SolverResult findSolutionAStar(C* init, path& p);
	
	/*!
	 * Checks if there is a solution using a breadth-first search.
	 * \param init the initial configuration.
	 */	
	SolverResult isSolvableBreadthFirst(C* init);

	/*!
	 * Checks if there is a solution using a depth-first search.
	 * \param init the initial configuration.
	 */	
	SolverResult isSolvableDepthFirst(C* init);
	
	/*!
	 * Checks if there is a solution using a best-first search.
	 * \param init the initial configuration.
	 */
	SolverResult isSolvableBestFirst(C* init);
	
	/*!
	 * Delete each of the members of the path.
	 * Does not modify the path.
	 */
	static void deletePath(const path& p);
	
	/*!
	 * Set m_keepSolving to true;
	 */
	void go() { m_keepSolving = true; }
	/*!
	 * Set m_keepSolving to false;
	 */
	void stop() { m_keepSolving = false; }
private:
	/*!
	 * Finds a solution if there is one, putting nodes into config_queue with push and choosing
	 * the next element to expand with next.
	 * For consistency of memory management, init will be put on p regardless.
	 * \param config_queue the queue into which configurations will be expanded.
	 * \param next a member function of config_queue which takes no arguments are returns the 
	 * next element to be expanded.
	 * \param init the initial configuration.
	 * \param p the path to add the solution to.
	 * \return true if a solution was found.
	 */
	template<class config_queue,const typename config_queue::value_type& (config_queue::* next) () const>
	SolverResult findSolution_g(C* init, path& p);
	
	/*!
	 * Checks if there is a solution using the provided solution.
	 * \param solve the member function to use to find the solution.
	 * \param init the initial configuration.
	 */
	template<SolverResult (Solver::* solve) (C*, path&)>
	SolverResult isSolvable_g(C* init);
	
	/*!
	 * A flag which threads can use to interrupt the solver.
	 */
	bool m_keepSolving;
};

/* ***************************************************************************
 * COMPARISON CLASSES
 * ***************************************************************************/

/*!
 * A comparison class which compares by estimated distance.
 */
template<class C>
class CompareByDistance
{
public:
	bool inline operator() (const C* c0, const C* c1) const {
		return c0->getEstimatedDistance() < c1->getEstimatedDistance();
	};
};

/*!
 * A comparison class which compares by contents.
 */
template<class C>
class CompareConfigurationPointers
{
public:
	bool inline operator() (const C* c0, const C* c1) const {
		return (*c0) < (*c1);
	};
};

/* ***************************************************************************
 * TEMPLATE IMPLEMENTATIONS
 * ***************************************************************************/

template<class C>
SolverResult Solver<C>::findSolution(SearchType type, C* init, typename Solver<C>::path& p)
{
	switch (type) {
		case(BREADTH_FIRST):
			return findSolutionBreadthFirst(init,p);
		case(DEPTH_FIRST):
			return findSolutionDepthFirst(init,p);
		case(BEST_FIRST):
			return findSolutionBestFirst(init,p);
		case(A_STAR):
			return findSolutionAStar(init,p);
	}
	assert(false);
}

/* **************************************************************************
 * findSolution_g
 * **************************************************************************/

template<class C> template<class config_queue, const typename config_queue::value_type& (config_queue::* next) () const>
SolverResult Solver<C>::findSolution_g(C* init, Solver<C>::path& p)
{
	// Push the initial configuration onto the path regardless.
	p.push_back(init);
	
	// the value we will return.
	SolverResult ret = NO_SOLUTION;
	
	// the "set" of nodes we've encountered so far stores parent pointers. 
	typedef std::map<C*, C*, CompareConfigurationPointers<C> > configuration_set;
	// the Configurations we've encountered so far.
	configuration_set encountered;
	// We've encountered the initial Configuration.
	encountered[init] = 0;
	
	// the paths we're considering.
	config_queue current_configs;
	// the first path is the path containing just the initial Configuration.
	current_configs.push(init);
	
	// a vector for putting neighbours in.
	std::vector<C*> neighbours;
	
	// a pointer to the path at the front of the queue.
	C* top_config;	
	
	// keep iterating until we've found a solution or we've tried all paths.
	while (m_keepSolving && !current_configs.empty()) {
		top_config = (current_configs.*next)();
		// we're now considering the top_path, so we can remove it from current_paths.
		current_configs.pop();
		
		// we've found a winning path.
		if (top_config->isGoal()) {	
			// trace back through parent pointers to obtain the winning path.
			path reverse_path;
			for (C* c = top_config; c != init; c = encountered[c]) 
				reverse_path.push_back(c);
			
			// Add the winning path to the provided path
			for (typename std::vector<C*>::reverse_iterator i = reverse_path.rbegin(); i != reverse_path.rend(); ++i) {
				p.push_back(*i);
				// ensure we don't delete these configs.
				encountered.erase(*i);
			}
			ret = FOUND_SOLUTION;
			break;
		}
		
		// iterate over its the neighbours.
		top_config->getNeighbours(neighbours);
		for(typename std::vector<C*>::iterator i = neighbours.begin(); i != neighbours.end(); ++i) {
			// only use neighbours not yet encountered.	
			if (encountered.find(*i) == encountered.end()) {
				// put the new node in the encountered set, logging its parent.
				encountered[*i] = top_config;
				current_configs.push(*i);
			} else 
				delete *i;
		}
		
		neighbours.clear();
	}

	if (!m_keepSolving)
		ret = INTERRUPTED;
	
	// Ensure we don't erase init.
	encountered.erase(init);
	// delete all Configurations still contained in the encountered set.
	for (typename configuration_set::iterator i = encountered.begin(); i != encountered.end(); ++i)
		delete (*i).first;
	
	return ret;
}

/* **************************************************************************
 * isSolvable_g
 * **************************************************************************/

template<class C> template<SolverResult (Solver<C>::* solve) (C*, typename Solver<C>::path&)>
SolverResult Solver<C>::isSolvable_g(C* init)
{
	path p;
	SolverResult ret = (this->*solve)(init,p);
	// delete configurations in the path except init (always first).
	for (unsigned int i = 1; i < p.size(); ++i)
		delete p[i];
	return ret;
}

/* **************************************************************************
 * instantiate findSolution_g
 * **************************************************************************/

template<class C>
SolverResult Solver<C>::findSolutionBreadthFirst(C* init, typename Solver<C>::path& p) 
{
	// use a queue to obtain a standard breadth-first search.
	// Newly expanded nodes are pushed at the back.
	// The front is the oldest node (i.e. the one with a shortest path so far).
	return findSolution_g<std::queue<C*> , &std::queue<C*>::front>(init, p);
}

template<class C>
SolverResult Solver<C>::findSolutionBestFirst(C* init, typename Solver<C>::path& p) 
{
	// use a priority_queue to obtain a greedy search.
	// Newly expanded nodes are pushed at the back.
	// The front is the node with the highest priority (i.e. the lowest getEstimatedDistance()).
	typedef std::priority_queue<C*, std::vector<C*>, CompareByDistance<C> > config_queue;
	return findSolution_g<config_queue , &config_queue::top>(init, p);
}


template<class C>
SolverResult Solver<C>::findSolutionDepthFirst(C* init, typename Solver<C>::path& p) 
{
	return findSolution_g<std::stack<C*> , &std::stack<C*>::top>(init, p);
}

/* **************************************************************************
 * instantiate isSolvable_g
 * **************************************************************************/


template<class C>
SolverResult Solver<C>::isSolvableBreadthFirst(C* init)
{
	return isSolvable_g<&Solver<C>::findSolutionBreadthFirst>(init);
}

template<class C>
SolverResult Solver<C>::isSolvableDepthFirst(C* init)
{
	return isSolvable_g<&Solver<C>::findSolutionDepthFirst>(init);
}

template<class C>
SolverResult Solver<C>::isSolvableBestFirst(C* init)
{
	return isSolvable_g<&Solver<C>::findSolutionBestFirst>(init);
}

template<class C>
void Solver<C>::deletePath(const path& p)
{
	for(typename path::const_iterator i = p.begin(); i != p.end(); ++i)
		delete *i; 
}

/* **************************************************************************
 * A* code
 * **************************************************************************/

/*!
 * Used to store additional information with the configurations in A*.
 */
template<class C>
class Node
{
public:
	/*!
	 * A pointer to the configuration managed by this node.
	 */
	C* config;
	
	/*!
	 * The minimal cost of reaching this node from the root.
	 */
	float g;
	
	/*!
	 * An estimate of how far we must travel to find the goal.
	 */
	float h;
	
	/*!
	 * A link to the parent configuration of this node along the minimal path.
	 */
	C* parent;
	
	/*!
	 * The constructor calls getEstimatedDistance only once so if necessary
	 * it can be quite slow.
	 */
	Node (C* cc, float gg, C* p) : config(cc), g(gg), h(cc->getEstimatedDistance()), parent(p) { }
};

/*!
 * A comparison class which ensures that nodes with the lowest f value are put at
 * the front of the heap.
 */
template<class C>
class CompareNodePointersByF
{
public:
	inline bool operator() (const Node<C>* n0, const Node<C>* n1) const {
		return n1->g + n1->h < n0->g + n0->h;
	};	
};

template<class C>
SolverResult Solver<C>::findSolutionAStar(C* init, typename Solver<C>::path& p) 
{
	typedef std::map<C*,C*,CompareConfigurationPointers<C> > configuration_set;
	typedef std::vector<Node<C>*> node_heap;
	typedef std::map<C*,Node<C>*,CompareConfigurationPointers<C> > node_map;
	CompareNodePointersByF<C> cmp;
	
	// the value we will return.
	SolverResult ret = NO_SOLUTION;
	
	// the set of nodes we have already explored.
	configuration_set closed_set;
	
	// The open set is the set of nodes we know about but haven't explored yet.
	// We manage this in two redundant ways:
	// We will maintain it as a heap with respect to f values.
	node_heap open_heap;
	// And also as a configuration -> node mapping.
	node_map open_map;
	
	// the initial node.
	Node<C>* init_n = new Node<C>(init,0.0,0);
	open_heap.push_back(init_n);
	open_map[init] = init_n;
	
	// used to point to the node with least f.
	Node<C>* top_node;
	// used for the set of neighbours.
	std::vector<C*> neighbours;
	
	while (!open_heap.empty() && m_keepSolving) {
		// the front of the open_set is the one with the lowest f.
		top_node = open_heap.front();
		// we've expanded top_node, so we can move it from the open_set to the closed_set.
		std::pop_heap<typename node_heap::iterator,CompareNodePointersByF<C> > (open_heap.begin(), open_heap.end(), cmp);
		open_heap.pop_back();
		open_map.erase(top_node->config);
		closed_set[top_node->config] = top_node->parent;
		
		// have we found the winning node?
		if (top_node->config->isGoal()) {
			// trace back through parent pointers to obtain the winning path.
			path reverse_path;
			for (C* c = top_node->config; c != 0; c = closed_set[c]) 
				reverse_path.push_back(c);
			
			// Add the winning path to the provided path
			for (typename std::vector<C*>::reverse_iterator i = reverse_path.rbegin(); i != reverse_path.rend(); ++i) {
				p.push_back(*i);
				// ensure we don't delete these configs.
				closed_set.erase(*i);
			}	
			delete top_node;
			ret = FOUND_SOLUTION;
			break;
		}
		
		// get the neighbouring configurations and iterate over them.
		top_node->config->getNeighbours(neighbours);
		for (typename std::vector<C*>::iterator n = neighbours.begin(); n != neighbours.end(); ++n) {
			// Only consider nodes not already in the closed set.
			if (closed_set.find(*n) == closed_set.end()) {
				// see if there's a node with this configuration already in the open set.
				typename node_map::iterator old = open_map.find(*n);		
				// if there is
				if (old != open_map.end()) {		
					// If we can improve the old node, do.
					if (top_node->g + 1 < (*old).second ->g) {
						// update old with the new g and parent.
						(*old).second->g = top_node->g + 1;
						(*old).second->parent = top_node->config;
						// remake the heap.
						std::make_heap<typename node_heap::iterator,CompareNodePointersByF<C> > (open_heap.begin(), open_heap.end(),cmp);
					}
					delete *n;
				} else {
					// add a new_node to the open_set.
					Node<C>* new_node = new Node<C>(*n,top_node->g + 1,top_node->config);
					open_heap.push_back(new_node);
					std::push_heap<typename node_heap::iterator, CompareNodePointersByF<C> > (open_heap.begin(), open_heap.end(), cmp);
					open_map[*n] = new_node;				
				}
			} else
				delete *n;
		}
		delete top_node;
		// clear the neighbours vector.
		neighbours.clear();	
	} 
	
	// if we haven't found a solution.
	if (ret != FOUND_SOLUTION) {
		p.push_back(init);
		// ensure we don't delete init.
		closed_set.erase(init);
	}
	
	if (!m_keepSolving)
		ret = INTERRUPTED;
	
	// Erase the remaining configurations in the closed set.
	for (typename configuration_set::iterator i = closed_set.begin(); i != closed_set.end(); ++i) {
		delete (*i).first;
	}
	
	// Erase the remaining nodes and configurations open set.
	for (typename node_heap::iterator i = open_heap.begin(); i != open_heap.end(); ++i) {
		delete (*i)->config;
		delete *i;
	}
	
	return ret;
}

} // namespace mzmslv

#endif /*SOLVER_H_*/
