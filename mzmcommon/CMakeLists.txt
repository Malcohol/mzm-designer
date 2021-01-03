SET( OBSERVER_SRCS
	Observer/Observer.cpp 
   )

SET( OPTIONS_SRCS 
	Options/Options.cpp 
   )

SET( MZMCOMMON_SRCS
	${OBSERVER_SRCS}
	${OPTIONS_SRCS}
   )

ADD_LIBRARY( mzmcommon ${MZMCOMMON_SRCS} )
