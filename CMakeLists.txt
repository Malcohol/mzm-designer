CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

###############################################################################
# Build the project
###############################################################################

PROJECT( MAZEZAM )

# You can uncomment the following to build with shared libraries.
# SET( BUILD_SHARED_LIBS TRUE )

SET( CMAKE_EDIT_COMMAND /usr/bin/cmake -i )
SET( ECLIPSE_CDT4_GENERATE_SOURCE_PROJECT TRUE )

INCLUDE_DIRECTORIES( ${MAZEZAM_SOURCE_DIR} )

ADD_SUBDIRECTORY( mzmcommon )
ADD_SUBDIRECTORY( mzmslv )
ADD_SUBDIRECTORY( mzm )
ADD_SUBDIRECTORY( mzm-solve )

FIND_PACKAGE( Curses )

IF( ${CURSES_FOUND} MATCHES "TRUE" )

	ADD_SUBDIRECTORY( mzm-designer )
	
ELSE( ${CURSES_FOUND} MATCHES "TRUE" )

	MESSAGE( WARNING "Curses not found, cannot build mzm-designer" )

ENDIF( ${CURSES_FOUND} MATCHES "TRUE" )

###############################################################################
# Generate the documentation
###############################################################################

FIND_PACKAGE( Doxygen )

IF( ${DOXYGEN_FOUND} MATCHES "YES" )

	SET( DOXYGEN_SOURCE_DIR ${MAZEZAM_SOURCE_DIR} )
	SET( DOXYGEN_OUTPUT_DIR docs )
	
	STRING( REGEX REPLACE ";" " " CMAKE_DOXYGEN_INPUT_LIST "${DOXYGEN_SOURCE_DIR}" )
	
	CONFIGURE_FILE( Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile )
	
	SET( HTML_TARGET “html” )
	
	
	ADD_CUSTOM_TARGET( ${HTML_TARGET} ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile )
	
	#INSTALL( DIRECTORY “${CMAKE_CURRENT_BINARY_DIR}/html/” DESTINATION “/usr/share/doc/libnoise-${LIBNOISE_VERSION}” )
	
ELSE( ${DOXYGEN_FOUND} MATCHES "YES" )

	MESSAGE( WARNING "Doxygen not found, cannot create documentation" )

ENDIF( ${DOXYGEN_FOUND} MATCHES "YES" )

