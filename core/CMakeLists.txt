
set(HEADER_LIST d_string.h d_string_list.h dp_list.h core.h min_heap.h)
set(SRC_LIST ${HEADER_LIST} d_string.c dp_list.c d_string_list.c)


add_library(core ${SRC_LIST})
set_target_properties(core PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(core PROPERTIES VERSION 1.0 SOVERSION 1)

## install(FILES ${HEADER_LIST} DESTINATION include)

