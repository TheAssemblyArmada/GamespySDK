add_library(gsqr2 OBJECT)
target_sources(gsqr2 PRIVATE
    qr2.c
    qr2.h
    qr2regkeys.c
    qr2regkeys.h
)

target_link_libraries(gsqr2 PUBLIC gsinterface)
set_target_properties(gsqr2 PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
