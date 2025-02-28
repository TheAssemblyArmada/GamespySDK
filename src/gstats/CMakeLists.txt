add_library(gsgstats OBJECT)
target_sources(gsgstats PRIVATE
    gbucket.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gstats/gbucket.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/gstats/gpersist.h
    gstats.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gstats/gstats.h
)

target_link_libraries(gsgstats PUBLIC gsinterface)
set_target_properties(gsgstats PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
