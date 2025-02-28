add_library(gsqr OBJECT)
target_sources(gsqr PRIVATE
gqueryreporting.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/qr/gqueryreporting.h
)

target_link_libraries(gsqr PUBLIC gsinterface)
set_target_properties(gsqr PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
