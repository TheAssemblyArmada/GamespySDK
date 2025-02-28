add_library(gsgp OBJECT)
target_sources(gsgp PRIVATE
    gp.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gp.h
    gpi.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpi.h
    gpibuddy.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpibuddy.h
    gpibuffer.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpibuffer.h
    gpicallback.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpicallback.h
    gpiconnect.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpiconnect.h
    gpiinfo.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpiinfo.h
    gpikeys.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpikeys.h
    gpioperation.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpioperation.h
    gpipeer.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpipeer.h
    gpiprofile.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpiprofile.h
    gpips3.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpips3.h
    gpisearch.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpisearch.h
    gpitransfer.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpitransfer.h
    gpiunique.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpiunique.h
    gpiutility.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp/gpiutility.h
)

target_link_libraries(gsgp PUBLIC gsinterface)
set_target_properties(gsgp PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
