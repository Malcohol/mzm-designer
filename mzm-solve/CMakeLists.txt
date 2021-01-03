SET( MZM_SOLVE_SRCS
	solve.cpp
	SolverOptions.cpp
   )

ADD_EXECUTABLE( mzm-solve ${MZM_SOLVE_SRCS} )

TARGET_LINK_LIBRARIES( mzm-solve mzmcommon mzm mzmslv pthread )
