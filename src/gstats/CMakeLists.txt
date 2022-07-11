add_library(gsgstats OBJECT)
target_sources(gsgstats PRIVATE
    gbucket.c
    gbucket.h
    gpersist.h
    gstats.c
    gstats.h
)

target_link_libraries(gsgstats PUBLIC gsinterface)
set_target_properties(gsgstats PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
