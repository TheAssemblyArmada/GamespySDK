add_library(gspinger OBJECT)
target_sources(gspinger PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/pinger/pinger.h
    pingermain.c
)

target_link_libraries(gspinger PUBLIC gsinterface)
set_target_properties(gspinger PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
