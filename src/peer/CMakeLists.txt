add_library(gspeer OBJECT)
target_sources(gspeer PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peer.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peerascii.h
    peerautomatch.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peerautomatch.h
    peercallbacks.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peercallbacks.h
    peerglobalcallbacks.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peerglobalcallbacks.h
    peerhost.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peerhost.h
    peerkeys.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peerkeys.h
    peermain.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peermain.h
    peermangle.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peermangle.h
    peeroperations.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peeroperations.h
    peerping.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peerping.h
    peerplayers.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peerplayers.h
    peerqr.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peerqr.h
    peerrooms.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peerrooms.h
    peersb.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/peer/peersb.h
)

target_link_libraries(gspeer PUBLIC gsinterface)
set_target_properties(gspeer PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
