add_library(gspt OBJECT)
target_sources(gspt PRIVATE
    pt.h
    ptmain.c
)

target_link_libraries(gspt PUBLIC gsinterface)
set_target_properties(gspt PROPERTIES POSITION_INDEPENDENT_CODE ON)
