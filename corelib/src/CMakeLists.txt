
SET(SRC_LIST d_string.c d_string.h dp_list.c dp_list.h d_string_list.c d_string_list.h corelib.h)


ADD_LIBRARY(corelib SHARED ${SRC_LIST})
SET_TARGET_PROPERTIES(corelib PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(corelib PROPERTIES VERSION 1.0 SOVERSION 1)

ADD_LIBRARY(corelib_static STATIC ${SRC_LIST})
SET_TARGET_PROPERTIES(corelib_static PROPERTIES OUTPUT_NAME "corelib")
SET_TARGET_PROPERTIES(corelib_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS corelib corelib_static  
  LIBRARY DESTINATION lib  
  ARCHIVE DESTINATION lib)

INSTALL(FILES corelib.h d_string.h d_string_list.h dp_list.h min_heap.h DESTINATION include)

