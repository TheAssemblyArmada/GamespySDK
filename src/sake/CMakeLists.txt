add_library(gssake OBJECT)
target_sources(gssake PRIVATE
    sake.h
    sakemain.c
    sakemain.h
    sakerequest.c
    sakerequest.h
    sakerequestinternal.h
    sakerequestmisc.c
    sakerequestmodify.c
    sakerequestread.c
)

target_link_libraries(gssake PUBLIC gsinterface)
set_target_properties(gssake PROPERTIES POSITION_INDEPENDENT_CODE ON)
