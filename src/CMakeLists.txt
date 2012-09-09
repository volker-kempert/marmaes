message("++ Process directory : src")
message("++ QT Libraries      : ${QT_LIBRARIES}")
message("++ Indclude Dirs     : ${includeDirs}")


set (MARMEAS_VERSION \"${V_MAJOR}.${V_MINOR}.${V_PATCH}\")
set (MARMEAS_PLUGIN_DIR ${CMAKE_INSTALL_PREFIX}/${MARMEAS_PLUGIN_INSTALL_DIR} )
set (MARMEAS_HELP_DIR   ${CMAKE_INSTALL_PREFIX}/${MARMEAS_HELP_INSTALL_DIR})
					 
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/injected_defines.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/injected_defines.h"
)


add_subdirectory(marmeas_gui)
add_subdirectory(marmeas_core)


include_directories( ${includeDir} 	
					 ${CMAKE_CURRENT_SOURCE_DIR} 
					 ${CMAKE_CURRENT_BINARY_DIR} )
					 
set (MARMEASLIB_SRCS
	arbitrary_math.cpp
    object_plugin.cpp      
    status_and_history.cpp
)

set(MARMEASLIB_MOC_SRCS
    status_and_history.h    
)

set (MARMEAS_SRCS
    help.cpp  
    help_browser.cpp
    image_context/image_context_dialog.cpp      
    image_context/image_context_templates_dialog.cpp  
    image_context/image_context_view.cpp  
    main.cpp  
    main_interaction.cpp
     
    object/object_data_dialog.cpp 
    object/object_data_view.cpp  
    object/object_template.cpp
    
    3rdparty/flowlayout/flowlayout.cpp    
    3rdparty/plotter/simple_plotter.cpp
    )

set(MARMEAS_MOC_SRCS
    help.h       
	image_context/image_context_dialog.h 
    image_context/image_context_templates_dialog.h 
    image_context/image_context_view.h 
    main_interaction.h
             
    object/object_data_dialog.h
    object/object_data_view.h  
    object/object_template.h
      
    3rdparty/flowlayout/flowlayout.h    
    3rdparty/plotter/simple_plotter.h
 )

set (no_coms
	basic_type_read_write.h
     
    graphics_point_item.h 
    image_context_read_write.h
    image_context_template.h
    object_base_read_write.h
    object_plugin.h        
    xml_reader_writer.h
    
    ${CMAKE_CURRENT_BINARY_DIR}/version.h         
)
    
set (MARMEAS_UIS
    config_object_and_measure.ui 
    help.ui 
	image_context_templates.ui 
	image_context.ui
    main.ui
    object_data_edit.ui
    object_data_view.ui 
)

set (MARMEAS_RCCS
    resources.qrc
)

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_XML_LIB)
ADD_DEFINITIONS(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG) 
ADD_DEFINITIONS(-DQT_GUI_LIB) 
ADD_DEFINITIONS(-DQT_CORE_LIB)
ADD_DEFINITIONS( -DQT_SHARED)

#----------------------------------------------------------------------------

qt4_wrap_cpp(MARMEAS_MOCS ${MARMEAS_MOC_SRCS})
qt4_wrap_ui(MARMEAS_UIS_H ${MARMEAS_UIS})
qt4_wrap_cpp(MARMEAS_MOC_UI ${MARMEAS_UIS_H})
qt4_add_resources(MARMEAS_RCC_SRCS ${MARMEAS_RCCS})


qt4_wrap_cpp(MARMEASLIB_MOCS ${MARMEASLIB_MOC_SRCS})
add_library(marmeasbase SHARED ${MARMEASLIB_SRCS} ${MARMEASLIB_MOCS} )

add_executable(marmeas
	${MARMEAS_SRCS}
	${MARMEAS_MOCS}
	${MARMEAS_MOC_UI}
	${MARMEAS_RCC_SRCS}
	)

target_link_libraries(marmeas marmeasbase marmeasgui marmeascore ${QT_LIBRARIES})



if (WIN32)
	install  (TARGETS marmeas RUNTIME DESTINATION bin)
	install  (TARGETS marmeasbase DESTINATION lib)
endif (WIN32)

if (UNIX)
	install  (TARGETS marmeas RUNTIME DESTINATION bin)
	install  (TARGETS marmeasbase LIBRARY DESTINATION lib)	
endif (UNIX)
