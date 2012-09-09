# marmeas project cmake file for plugins
# inherits from the base configuration


message("++ Process directory : ${CMAKE_CURRENT_SOURCE_DIR}")
message("++ include dir  : ${includeDir}")

find_package(Qt4 REQUIRED)
include ( ${QT_USE_FILE})

add_subdirectory(src)
add_subdirectory(tests)



