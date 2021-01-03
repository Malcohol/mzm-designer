SET( MZM_DESIGNER_SRCS
	CursesWindow.cpp
	Designer.cpp
	DesignerOptions.cpp
	Editor.cpp
	Improver.cpp
	main.cpp
	MazezamCurses.cpp
	MazezamPlayable.cpp
	MazezamPlayback.cpp
   )

ADD_EXECUTABLE( mzm-designer ${MZM_DESIGNER_SRCS} )

TARGET_LINK_LIBRARIES( mzm-designer mzmcommon mzm mzmslv pthread curses )

