/* ***************************************************************************
 * MazezamImprover.hpp
 * Copyright (c) 2008, 2010, 2020 Malcolm Tyrrell.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://gplv3.fsf.org/
 * ***************************************************************************/

#ifndef MAZEZAMIMPROVER_H_
#define MAZEZAMIMPROVER_H_

#include <mzm/Mazezam/MazezamData.hpp>
#include <mzmslv/WorkerPool.hpp>
#include <mzm/MazezamSolver/MazezamRatingType.hpp>

namespace mzm {

class Selection;
class SolutionCollector;

/*!
 * This classes uses a worker pool to find the "best" neighbour
 * of the given mazezam.
 */
class MazezamImprover : public mzmslv::WorkerPoolClient
{
public:
	/*!
	 * Constructor from a MazezamBase.
	 * \param m the mazezam to improve
	 * \param radius the maximum number of blocks to consider changing.
	 */
	MazezamImprover(const MazezamData& m, MazezamRating startRating, mzmslv::WorkerPool& workerPool, SolutionCollector& collector, unsigned int radius, const Selection* selection = NULL);
	/*!
	 * Destructor.
	 */
	virtual ~MazezamImprover();
	/*!
 	 * Find the highest rated neighbour of this mazezam.
 	 * \return true if the process found a higher rated mazezam.
 	 */
 	bool improve();
	/*!
	 * Start improving the mazezam.
	 */
	void improveAsynchronously();
	/*!
	 * Has the improvement processed finished trying neighbours?
	 */
	bool isFinished() const;
	/*!
	 * Stop an asynchronous improvement.
	 */
	void stop();
	/*!
	 * Obtain the best level found, or the start level
	 * if no neighbours were better.
	 */
	const MazezamData& getBestLevel() const;
	
	// Implementation of WorkerPoolClient interface.
	virtual mzmslv::WorkerPoolJob* getNextJob();
	virtual void jobDone(mzmslv::WorkerPoolJob* job);
protected:
	/*!
	 * The starting level.
	 */
	const MazezamData& m_level;
	/*!
	 * The collector to which we pass solutions. 
	 */
	SolutionCollector& m_collector;
	/*!
	 * The worker pool to use for improvement.
	 */
	mzmslv::WorkerPool& m_workerPool;
	/*!
	 * Those positions which should be considered for improvement.
	 */
	const Selection* m_selection;
	/*!
	 * The maximum number of blocks to consider changing.
	 */
	unsigned int m_radius;
	/*!
	 * A neighbour is described as a set of blocks to flip.
	 * This is strictly increasing and has a size less than or
	 * equal to m_depth.
	 */
	std::vector<int> m_changes;
	/*!
	 * The best level so far.
	 */
	MazezamData m_bestLevel;
	/*!
	 * The best rating so far.
	 */
	MazezamRating m_bestRating;
private:
	/*!
	 * The index that represents the position in front of the entrance.
	 */
	int indexOfEntrance() const;
	/*!
	 * Get the index after current.
	 */
	int getNextIndex(int current) const;
	/*!
	 * Is the index the greatest valid change for its position in the
	 * changes array?
	 */
	bool advanceIndex(unsigned int pos);
	/*!
	 * Set the indices to pos to the lowest unique values they can
	 * hold.
	 */
	void resetIndicesToPos(unsigned int pos);
	/*!
	 * Advance the neighbour description.
	 * \return false if there are no more neighbours.
	 */
	bool advanceNeighbour();
	
	/*!
	 * Creates a config.
	 * Note: The caller takes responsibility for deleting the underlying level.
	 */
	MazezamData* getNeighbourLevel() const;
	
	//
	// Private and unimplemented methods.
	//
	
	/*!
	 * Copy constructor (private and unimplemented).
	 */
	MazezamImprover (const MazezamImprover& other);
	/*!
	 * Assignment (private and unimplemented).
	 */
	MazezamImprover& operator= (const MazezamImprover& other);	
};

} // namespace mzm

#endif /*MAZEZAMIMPROVER_H_*/
