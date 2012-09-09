project(MARMEAS)
cmake_minimum_required(VERSION 2.8.0)

set (CMAKE_CXX_FLAGS_INIT "")
set (CMAKE_CXX_FLAGS_DEBUG_INIT "-g")

set(V_MAJOR 0 CACHE STRING "Major Version?")
set(V_MINOR 0 CACHE STRING "Minor Version?")
set(V_PATCH 0 CACHE STRING "Patch Version?")
set(V_BUILD 0 CACHE STRING "Build Number?")

set(MARMEAS_PLUGIN_INSTALL_DIR  "lib/marmeas-plugin" CACHE STRING "Plugin Install Dir")

set(VERSION = ${V_MAJOR}.${V_MINOR})
if (UNIX)
	set(MARMEAS_HELP_INSTALL_DIR "share/doc/marmeas")
endif (UNIX)
if (WIN32)
	set(MARMEAS_HELP_INSTALL_DIR "doc")
endif (WIN32)

set ( ORGANISATION pixmeter )

message("++ Generator:        ${CMAKE_GENERATOR}")
message("++ Compiler:         ${CMAKE_CXX_COMPILER}")
message("++ Target OS:        ${CMAKE_SYSTEM_NAME}")
message("++ Target Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message("++ Build Type:       ${CMAKE_BUILD_TYPE}")

include(UseDoxygen OPTIONAL)

set (QT_USE_QTXML TRUE)
set (QT_USE_QTHELP TRUE)
set (QT_USE_QTSQL FALSE)
set (QT_USE_QTNETWORKS FALSE)

find_package(Qt4 REQUIRED)
message("++ QT Libraries:       ${QT_LIBRARIES}")
message("++ IS Unix     : ${UNIX}")
message("++ IS Windows  : ${WIN32}")



set(includeDir 
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/marmeas_core
	${CMAKE_SOURCE_DIR}/include/marmeas_gui
	${CMAKE_SOURCE_DIR}/src  # allow direct access for unit testing
	)

message("++ QT_USE_FILE      : ${QT_USE_FILE}")
include ( ${QT_USE_FILE} )
message("++ QT Libraries      : ${QT_LIBRARIES}")


#------------------------------------------------------------------------------
#  section: install
#------------------------------------------------------------------------------

if (UNIX)
	set(CMAKE_INSTALL_PREFIX "/usr")
	
	# create appropriated desktop entries 
	if(  NOT APPLE )
	    if( NOT DESKTOP_ENTRY )
	      set( DESKTOP_ENTRY marmeas.desktop )
	    endif( NOT DESKTOP_ENTRY )
	  
	    add_custom_command( OUTPUT ${DESKTOP_ENTRY}
	      COMMAND touch ${DESKTOP_ENTRY}
	      COMMAND sh ${CMAKE_SOURCE_DIR}/marmeas-desktop.sh ${CMAKE_INSTALL_PREFIX} ${VERSION}>${DESKTOP_ENTRY}
	      DEPENDS ${CMAKE_SOURCE_DIR}/marmeas-desktop.sh
	      COMMENT "Generating desktop entry file"
	      )
	    add_custom_target( DESKTOP_ENTRY_FILE ALL
	      DEPENDS ${DESKTOP_ENTRY}
	      )
	  
	    set( APP_ICON ${CMAKE_SOURCE_DIR}/src/resources/marmeas.png )
    	install( FILES ${APP_ICON} DESTINATION share/icons )
    	install( FILES ${CMAKE_BINARY_DIR}/${DESKTOP_ENTRY} DESTINATION share/applications/desktop )
    	install( FILES ${CMAKE_SOURCE_DIR}/marmeas.conf DESTINATION /etc/xdg/${ORGANISATION} )
	 endif( NOT APPLE )
endif (UNIX)

message("++ Install Prefix: ${CMAKE_INSTALL_PREFIX}")	
message("++ Plugin Install Dir: ${MARMEAS_PLUGIN_INSTALL_DIR}")		


#------------------------------------------------------------------------------
#  section: packaging
#------------------------------------------------------------------------------
#
# set(CPACK_SET_DESTDIR "OFF")

set(CPACK_SET_DESTDIR "ON")
set(CPACK_PACKAGE_NAME "marmeas")
SET(CPACK_PACKAGE_VERSION_MAJOR ${V_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${V_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${V_PATCH})
SET(CPACK_PACKAGE_BUILD ${V_BUILD})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "marking and measurement of objects on images")
set(CPACK_PACKAGE_VENDOR "marmeas development team")
#set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/license.txt)

	
if (UNIX)
	SET(CPACK_SOURCE_GENERATOR TBZ2)
	SET(CPACK_GENERATOR DEB)

	#SET(CPACK_PACKAGE_INSTALL_DIRECTORY "/")
	set(CPACK_PACKAGING_INSTALL_PREFIX "usr")
	#set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
	#set(CPACK_PACKAGE_DEFAULT_LOCATION "/usr")
	
	set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS   "")
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Volker Kempert <volker.kempert@gmx.net>")
	# set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/package-description.txt)
	set(CPACK_DEBIAN_PACKAGE_SECTION "contrib/libs")
	SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-${CPACK_PACKAGE_VERSION_PATCH}.${CPACK_PACKAGE_BUILD}.${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})
endif (UNIX)
if (WIN32)
	SET (CPACK_GENERATOR NSIS)
	set (CPACK_MONOLITHIC_INSTALL 1)
	set (CPACK_SYSTEM_NAME win32)
	set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME} ${VERSION}")
	set (CPACK_EXECUTABLES "Icomm" "marmeas")

endif (WIN32)


#------------------------------------------------------------------------------
#  process the sub dirs
#------------------------------------------------------------------------------

add_subdirectory(plugins)
add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(tests)

include(CPack)

