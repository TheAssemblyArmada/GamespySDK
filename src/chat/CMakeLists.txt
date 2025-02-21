add_library(gschat OBJECT)
target_sources(gschat PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/chat.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/chatascii.h
    chatcallbacks.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chatcallbacks.h
    chatchannel.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chatchannel.h
    chatcrypt.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chatcrypt.h
    chathandlers.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chathandlers.h
    chatmain.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chatmain.h
    chatsocket.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chatsocket.h
)

target_link_libraries(gschat PUBLIC gsinterface)
set_target_properties(gschat PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
