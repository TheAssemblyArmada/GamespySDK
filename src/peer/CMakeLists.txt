add_library(gspeer OBJECT)
target_sources(gspeer PRIVATE
    peer.h
    peerascii.h
    peerautomatch.c
    peerautomatch.h
    peercallbacks.c
    peercallbacks.h
    peerglobalcallbacks.c
    peerglobalcallbacks.h
    peerhost.c
    peerhost.h
    peerkeys.c
    peerkeys.h
    peermain.c
    peermain.h
    peermangle.c
    peermangle.h
    peeroperations.c
    peeroperations.h
    peerping.c
    peerping.h
    peerplayers.c
    peerplayers.h
    peerqr.c
    peerqr.h
    peerrooms.c
    peerrooms.h
    peersb.c
    peersb.h
)

target_link_libraries(gspeer PUBLIC gsinterface)
set_target_properties(gspeer PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
