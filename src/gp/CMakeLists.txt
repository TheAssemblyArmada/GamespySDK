add_library(gsgp OBJECT)
target_sources(gsgp PRIVATE
    gp.c
    gp.h
    gpi.c
    gpi.h
    gpibuddy.c
    gpibuddy.h
    gpibuffer.c
    gpibuffer.h
    gpicallback.c
    gpicallback.h
    gpiconnect.c
    gpiconnect.h
    gpiinfo.c
    gpiinfo.h
    gpikeys.c
    gpikeys.h
    gpioperation.c
    gpioperation.h
    gpipeer.c
    gpipeer.h
    gpiprofile.c
    gpiprofile.h
    gpips3.c
    gpips3.h
    gpisearch.c
    gpisearch.h
    gpitransfer.c
    gpitransfer.h
    gpiunique.c
    gpiunique.h
    gpiutility.c
    gpiutility.h
)

target_link_libraries(gsgp PUBLIC gsinterface)
set_target_properties(gsgp PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
