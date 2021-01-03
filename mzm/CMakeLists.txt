SET( Mazezam_SRCS
	Mazezam/MazezamData.cpp
	Mazezam/MazezamHistory.cpp
	Mazezam/MazezamPerson.cpp
	Mazezam/TypesAndConstants.cpp
   )

SET( MazezamSolver_SRCS
	MazezamSolver/BackgroundSolver.cpp
	MazezamSolver/Config.cpp
	MazezamSolver/ConfigEqv.cpp
	MazezamSolver/ConfigWalk.cpp
	MazezamSolver/MazezamImprover.cpp
	MazezamSolver/MazezamSolutionType.cpp
	MazezamSolver/MazezamSolverJob.cpp
	MazezamSolver/MultiMazezamSolver.cpp
	MazezamSolver/OfflineSolver.cpp
	MazezamSolver/OutstreamSolutionCollector.cpp
	MazezamSolver/Selection.cpp
   )

SET( MzmReaders_SRCS
	MzmReaders/FileMzmReader.cpp
	MzmReaders/SelectiveMzmReader.cpp
   )

SET( RangePred_SRCS
	RangePred/RangePred.cpp
   )

SET( mzm_SRCS
	${Mazezam_SRCS}
	${MazezamSolver_SRCS}
	${MzmReaders_SRCS}
	${RangePred_SRCS}
   )

ADD_LIBRARY( mzm ${mzm_SRCS} )
