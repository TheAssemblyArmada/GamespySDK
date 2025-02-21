add_library(gssake OBJECT)
target_sources(gssake PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/sake.h
    sakemain.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sakemain.h
    sakerequest.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sakerequest.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/sakerequestinternal.h
    sakerequestmisc.c
    sakerequestmodify.c
    sakerequestread.c
)

target_link_libraries(gssake PUBLIC gsinterface)
set_target_properties(gssake PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
