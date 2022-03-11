add_library(gsgt2 OBJECT)
target_sources(gsgt2 PRIVATE
    gsudpengine.c
    gsudpengine.h
    gt2.h
    gt2auth.c
    gt2auth.h
    gt2buffer.c
    gt2buffer.h
    gt2callback.c
    gt2callback.h
    gt2connection.c
    gt2connection.h
    gt2encode.c
    gt2encode.h
    gt2filter.c
    gt2filter.h
    gt2main.c
    gt2main.h
    gt2message.c
    gt2message.h
    gt2socket.c
    gt2socket.h
    gt2utility.c
    gt2utility.h
)

target_link_libraries(gsgt2 PUBLIC gsinterface)
set_target_properties(gsgt2 PROPERTIES POSITION_INDEPENDENT_CODE ON)
