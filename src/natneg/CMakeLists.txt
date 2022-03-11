add_library(gsnatneg OBJECT)
target_sources(gsnatneg PRIVATE
    natify.c
    natify.h
    natneg.c
    natneg.h
    nninternal.h
)

target_link_libraries(gsnatneg PUBLIC gsinterface)
set_target_properties(gsnatneg PROPERTIES POSITION_INDEPENDENT_CODE ON)
