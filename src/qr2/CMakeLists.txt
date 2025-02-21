add_library(gsqr2 OBJECT)
target_sources(gsqr2 PRIVATE
    qr2.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/qr2.h
    qr2regkeys.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/qr2regkeys.h
)

target_link_libraries(gsqr2 PUBLIC gsinterface)
set_target_properties(gsqr2 PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
