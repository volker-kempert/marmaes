PROJECT (marmeasline CXX C)

message("++ Process directory : ${CMAKE_CURRENT_SOURCE_DIR}")
message("++ QT InstallPlugins : ${MARMEAS_PLUGIN_INSTALL_DIR}")

include_directories( ${includeDir} 	
					 ${CMAKE_CURRENT_SOURCE_DIR} 
					 ${CMAKE_CURRENT_BINARY_DIR} )
					 


set (MARMEASPLUGIN_SRCS
    line.cpp 
    geometric_measurement_line.cpp
    measurement_line.cpp
    profile_measurement_line.cpp
    xml_line.cpp
    plugin_line.cpp  
    )


set (MARMEASPLUGIN_MOC_SRCS
    plugin_line.h
)	

set (MARMEASPLUGIN_RCCS
    line_resources.qrc
)


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_XML_LIB)
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

qt4_add_resources(MARMEASPLUGIN_RCC_SRCS ${MARMEASPLUGIN_RCCS})
qt4_wrap_cpp(MARMEASPLUGIN_MOCS ${MARMEASPLUGIN_MOC_SRCS})

message("++ MOCS : ${MARMEASPLUGIN_MOCS}")

add_library(marmeasline SHARED
	${MARMEASPLUGIN_SRCS}
	${MARMEASPLUGIN_MOCS}
	${MARMEASPLUGIN_RCC_SRCS}
	)

install (TARGETS marmeasline DESTINATION ${MARMEAS_PLUGIN_INSTALL_DIR})


