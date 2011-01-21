cmake_minimum_required(VERSION 2.8)

project(livecard)

### options ### 
# version
SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 0)

# global configure file
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

include_directories("${PROJECT_BINARY_DIR}")


# build output 
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

### libs ####
#SET(BUILD_SHARED_LIBS ON)
add_subdirectory(core)
add_subdirectory(dblayer)
add_subdirectory(utils)

### utils ####


### tests ####


### build ### 

### install ### 
# binaries 
#install(TARGETS core dblayer 
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib  
#  ARCHIVE DESTINATION lib)

# docs 
install(DIRECTORY doc/ DESTINATION share/doc/${PROJECT_NAME})
install(FILES COPYRIGHT README DESTINATION share/doc/${PROJECT_NAME})
