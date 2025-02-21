add_library(gsnatneg OBJECT)
target_sources(gsnatneg PRIVATE
    natify.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/natify.h
    natneg.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/natneg.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/nninternal.h
)

target_link_libraries(gsnatneg PUBLIC gsinterface)
set_target_properties(gsnatneg PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
