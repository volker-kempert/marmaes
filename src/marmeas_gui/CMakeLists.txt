#   Copyright (C) 2012, Volker Kempert <volker.kempert@gmx.net>
#
#   This file is part of an MARMEAS library
#
#   The MARMEAS library is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as
#   published by the Free Software Foundation, either version 3 of
#   the License, or (at your option) any later version.
#
#   The MARMEAS library is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public
#   License along with MARMEAS. If not, see <http://www.gnu.org/licenses/>.

message("++ Process directory : src/marmeas_gui")
message("++ QT Libraries      : ${QT_LIBRARIES}")
message("++ Indclude Dirs     : ${includeDirs}")


	 
include_directories( ${includeDir} 	
					 ${CMAKE_CURRENT_SOURCE_DIR} 
					 ${CMAKE_CURRENT_BINARY_DIR} )
					 
set (MARMEAS_GUI_SRCS
	graphics_item
    graphics_point_item.cpp
    graphics_scene.cpp
    graphics_view.cpp  
)

set(MARMEAS_GUI_MOC_SRCS
    graphics_view.h
    graphics_scene.h
)



ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_XML_LIB)
ADD_DEFINITIONS(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG) 
ADD_DEFINITIONS(-DQT_GUI_LIB) 
ADD_DEFINITIONS(-DQT_CORE_LIB)
ADD_DEFINITIONS( -DQT_SHARED)

qt4_wrap_cpp(MARMEAS_GUI_MOCS ${MARMEAS_GUI_MOC_SRCS})
add_library(marmeasgui SHARED ${MARMEAS_GUI_SRCS} ${MARMEAS_GUI_MOCS} )

#----------------------------------------------------------------------------


if (WIN32)
	install (TARGETS marmeasgui DESTINATION lib)
endif (WIN32)

if (UNIX)
	install (TARGETS marmeasgui LIBRARY DESTINATION lib)
endif (UNIX)
