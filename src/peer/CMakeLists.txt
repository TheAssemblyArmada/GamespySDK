add_library(gspeer OBJECT)
target_sources(gspeer PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/peerascii.h
    peerautomatch.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peerautomatch.h
    peercallbacks.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peercallbacks.h
    peerglobalcallbacks.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peerglobalcallbacks.h
    peerhost.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peerhost.h
    peerkeys.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peerkeys.h
    peermain.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peermain.h
    peermangle.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peermangle.h
    peeroperations.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peeroperations.h
    peerping.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peerping.h
    peerplayers.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peerplayers.h
    peerqr.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peerqr.h
    peerrooms.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peerrooms.h
    peersb.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peersb.h
)

target_link_libraries(gspeer PUBLIC gsinterface)
set_target_properties(gspeer PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
