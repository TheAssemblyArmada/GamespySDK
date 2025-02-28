add_library(gspt OBJECT)
target_sources(gspt PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/pt/pt.h
    ptmain.c
)

target_link_libraries(gspt PUBLIC gsinterface)
set_target_properties(gspt PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
