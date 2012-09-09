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


message("++ Process directory : src/marmeas_core")
message("++ QT Libraries      : ${QT_LIBRARIES}")
message("++ Indclude Dirs     : ${includeDirs}")


	 
include_directories( ${includeDir} 	
					 ${CMAKE_CURRENT_SOURCE_DIR} 
					 ${CMAKE_CURRENT_BINARY_DIR} )
					 
set (MARMEAS_CORE_SRCS
   
    core/basic_type_read_write.cpp
    core/xml_reader_writer.cpp    	
    
    image_context/image_context.cpp
    image_context/image_context_template.cpp    
    image_context/image_context_read_write.cpp      	
    	
	measurement/composite_measurement.cpp   	
	measurement/geometric_measurement.cpp   
	measurement/intensity_measurement.cpp  	
	measurement/histogram_statistics.cpp
	measurement/measurement_on_object.cpp 
    measurement/measurement_read_write.cpp
    measurement/point_clowd.cpp
	measurement/quartile.cpp
	    
    object/object_base.cpp  
    object/object_base_read_write.cpp
    object/geometric_object_container.cpp      
)

set(MARMEAS_CORE_MOC_SRCS
    ../../include/marmeas_core/geometric_object.h 
    ../../include/marmeas_core/geometric_object_container.h
	../../include/marmeas_core/measurement_on_object.h
	
    image_context/image_context.h    
	measurement/quartile.h    
)


set (no_coms

)
    


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_XML_LIB)
ADD_DEFINITIONS(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG) 
ADD_DEFINITIONS(-DQT_NO_GUI_LIB) 
ADD_DEFINITIONS(-DQT_CORE_LIB)
ADD_DEFINITIONS( -DQT_SHARED)

qt4_wrap_cpp(MARMEAS_CORE_MOCS ${MARMEAS_CORE_MOC_SRCS})
add_library(marmeascore SHARED ${MARMEAS_CORE_SRCS} ${MARMEAS_CORE_MOCS} )

#----------------------------------------------------------------------------


if (WIN32)
	install (TARGETS marmeascore DESTINATION lib)
endif (WIN32)

if (UNIX)
	install (TARGETS marmeascore LIBRARY DESTINATION lib)
endif (UNIX)
