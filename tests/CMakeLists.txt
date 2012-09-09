message("++ Process directory : tests")
message("++ Include Dirs : ${includeDir}")
	
include_directories( ${includeDir} 	
					 ${CMAKE_CURRENT_SOURCE_DIR} 
					 ${CMAKE_CURRENT_BINARY_DIR} )
					 
	
set(TEST_SRC 

	core/xml_basic_tests.cpp	
	
	image_context/image_context_tests.cpp
	image_context/xml_image_context_test.cpp

	measurement/composite_measurement_tests.cpp	
	measurement/geometric_measurement_tests.cpp
	measurement/histogram_statistics_tests.cpp
	measurement/intensity_measurement_tests.cpp
	measurement/measurement_tests.cpp
	measurement/xml_measurement_tests.cpp
		
	object/object_base_tests.cpp
	
	status_and_history_tests.cpp
	xml_full_tests.cpp

	)
	
		
set(MARMEAS_MOC_SRC_GUI
  
)

set(TEST_SRC_GUI 
	object/geometric_object_container_tests.cpp	

	)
 
set (MARMEASLIB
	marmeasbase  marmeascore marmeasgui
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

 	
add_executable(marmeas-cli-testrunner  testrunner.cpp ${TEST_SRC} ${MARMEAS_MOC} )
target_link_libraries(marmeas-cli-testrunner 
					  cppunit stdc++ ${QT_LIBRARIES} ${MARMEASLIB} pthread )
install  (TARGETS marmeas-cli-testrunner RUNTIME DESTINATION bin)


add_executable(marmeas-gui-testrunner  
				qtestrunner.cpp ${TEST_SRC} ${MARMEAS_MOC} 
				${TEST_SRC_GUI} ${MARMEAS_MOC_GUI} )
target_link_libraries(marmeas-gui-testrunner 
						cppunit stdc++ ${QT_LIBRARIES} ${MARMEASLIB} 
						pthread qxcppunitd           )
install  (TARGETS marmeas-gui-testrunner RUNTIME DESTINATION bin)
