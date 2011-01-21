set(HEADER_LIST dblayer.h)
set(SRC_LIST ${HEADER_LIST} dblayer.c)

include_directories(../core)

add_library(dblayer ${SRC_LIST})
set_target_properties(dblayer PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(dblayer PROPERTIES VERSION 1.0 SOVERSION 1)


#install(FILES {HEADER_LIST} DESTINATION include)
