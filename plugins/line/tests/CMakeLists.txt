message("++ Process directory : plugins/line")
message("++ Include Dirs : ${includeDir}")
	
include_directories( ${includeDir} 	
					 ${CMAKE_CURRENT_SOURCE_DIR} 
					 ${CMAKE_CURRENT_BINARY_DIR} )
					 
	
set(TEST_SRC 


	)
	
		
set(TEST_MOC_SRC_GUI
  
)

set(TEST_SRC_GUI 
	line_object_tests.cpp
	xml_line_object_tests.cpp
	)
 
set (MARMEASLIB
	marmeasbase marmeascore marmeasgui 
)

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_XML_LIB)
ADD_DEFINITIONS(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG) 
ADD_DEFINITIONS(-DQT_GUI_LIB) 
ADD_DEFINITIONS(-DQT_CORE_LIB)
ADD_DEFINITIONS( -DQT_SHARED)

qt4_wrap_cpp(MARMEAS_MOC ${MARMEAS_MOC_SRC})
qt4_wrap_cpp(MARMEAS_MOC_GUI ${MARMEAS_MOC_SRC_GUI})

 	
add_executable(line-cli-testrunner  testrunner.cpp ${TEST_SRC} ${MARMEAS_MOC} )
target_link_libraries(line-cli-testrunner   ${MARMEASLIB}
					  cppunit  stdc++  ${QT_LIBRARIES}   )
install  (TARGETS line-cli-testrunner RUNTIME DESTINATION bin)


add_executable(line-gui-testrunner  
				qtestrunner.cpp ${TEST_SRC} ${MARMEAS_MOC} 
				${TEST_SRC_GUI} ${MARMEAS_MOC_GUI} )
target_link_libraries(line-gui-testrunner ${MARMEASLIB} 
						cppunit stdc++ ${QT_LIBRARIES} 
						pthread qxcppunitd           )
install  (TARGETS line-gui-testrunner RUNTIME DESTINATION bin)
