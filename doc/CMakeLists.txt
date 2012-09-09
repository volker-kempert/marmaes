project(MARMEAS)
cmake_minimum_required(VERSION 2.8.0)

message("++ Process directory : doc")

set(docin_coll_in marmeas.qhcp.in)
set(docin_coll marmeas.qhcp)
set(docout_coll marmeas.qhc)
set(docin_helpfile marmeas.qhp)
set(docout_helpfile marmeas.qch)

set(docfiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/index.html
    ${CMAKE_CURRENT_SOURCE_DIR}/glossary.html
)

add_custom_command(
    OUTPUT ${docout_helpfile}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${docin_helpfile} ${docfiles} 
    COMMAND qhelpgenerator  ${CMAKE_CURRENT_SOURCE_DIR}/${docin_helpfile}
    ARGS -o  ${docout_helpfile}
    )

# this is required to force qcollectiongenerator to actually reference to 
# the correct qch file and still allow for out-bound builds

configure_file (
		"${CMAKE_CURRENT_SOURCE_DIR}/${docin_coll}"
    	"${CMAKE_CURRENT_BINARY_DIR}/${docin_coll}" COPYONLY
    	)

add_custom_command(
    OUTPUT ${docout_coll}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${docin_coll} 
    		${CMAKE_CURRENT_BINARY_DIR}/${docout_helpfile} 
    COMMAND qcollectiongenerator  ${CMAKE_CURRENT_BINARY_DIR}/${docin_coll}
    ARGS -o  ${docout_coll}
    )
  
    
ADD_CUSTOM_TARGET( docu_coll DEPENDS ${docout_coll} ${docout_helpfile}  )   
ADD_CUSTOM_TARGET( docu_helpfile DEPENDS ${docout_coll} ${docout_helpfile}  )   

add_dependencies(marmeas docu_coll)  
add_dependencies(docu_coll docu_helpfile)      
    

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${docout_coll} DESTINATION ${MARMEAS_HELP_INSTALL_DIR} )
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${docout_helpfile} DESTINATION ${MARMEAS_HELP_INSTALL_DIR} )
