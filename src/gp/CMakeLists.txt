add_library(gsgp OBJECT)
target_sources(gsgp PRIVATE
    gp.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gp.h
    gpi.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpi.h
    gpibuddy.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpibuddy.h
    gpibuffer.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpibuffer.h
    gpicallback.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpicallback.h
    gpiconnect.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpiconnect.h
    gpiinfo.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpiinfo.h
    gpikeys.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpikeys.h
    gpioperation.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpioperation.h
    gpipeer.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpipeer.h
    gpiprofile.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpiprofile.h
    gpips3.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpips3.h
    gpisearch.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpisearch.h
    gpitransfer.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpitransfer.h
    gpiunique.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpiunique.h
    gpiutility.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gpiutility.h
)

target_link_libraries(gsgp PUBLIC gsinterface)
set_target_properties(gsgp PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
