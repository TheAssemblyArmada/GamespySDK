add_library(gscdkey OBJECT)
target_sources(gscdkey PRIVATE
    gcdkeyc.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gcdkeyc.h
    gcdkeys.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gcdkeys.h
)

target_link_libraries(gscdkey PUBLIC gsinterface)
set_target_properties(gscdkey PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
