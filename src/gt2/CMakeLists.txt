add_library(gsgt2 OBJECT)
target_sources(gsgt2 PRIVATE
    gsudpengine.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gsudpengine.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2.h
    gt2auth.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2auth.h
    gt2buffer.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2buffer.h
    gt2callback.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2callback.h
    gt2connection.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2connection.h
    gt2encode.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2encode.h
    gt2filter.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2filter.h
    gt2main.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2main.h
    gt2message.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2message.h
    gt2socket.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2socket.h
    gt2utility.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gt2/gt2utility.h
)

target_link_libraries(gsgt2 PUBLIC gsinterface)
set_target_properties(gsgt2 PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
