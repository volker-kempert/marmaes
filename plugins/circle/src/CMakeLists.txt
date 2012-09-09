PROJECT (marmeascircle CXX C)

message("++ Process directory : ${CMAKE_CURRENT_SOURCE_DIR}")
message("++ QT InstallPlugins : ${MARMEAS_PLUGIN_INSTALL_DIR}")

include_directories( ${includeDir} 	
					 ${CMAKE_CURRENT_SOURCE_DIR} 
					 ${CMAKE_CURRENT_BINARY_DIR} )
					 

set (MARMEASPLUGIN_SRCS
    2point_circle.cpp      
    3point_circle.cpp
    circle.cpp
	geometric_measurement_circle.cpp  
	histogram_measurement_circle.cpp
    measurement_circle.cpp
    xml_circle.cpp
    plugin_circle.cpp  
    )


set (MARMEASPLUGIN_MOC_SRCS
    plugin_circle.h
)	

set (MARMEASPLUGIN_RCCS
    resources_circle.qrc
)


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_XML_LIB)
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

qt4_add_resources(MARMEASPLUGIN_RCC_SRCS ${MARMEASPLUGIN_RCCS})
qt4_wrap_cpp(MARMEASPLUGIN_MOCS ${MARMEASPLUGIN_MOC_SRCS})

message("++ MOCS : ${MARMEASPLUGIN_MOCS}")

add_library(marmeascircle SHARED
	${MARMEASPLUGIN_SRCS}
	${MARMEASPLUGIN_MOCS}
	${MARMEASPLUGIN_RCC_SRCS}
	)

install (TARGETS marmeascircle DESTINATION ${MARMEAS_PLUGIN_INSTALL_DIR})


