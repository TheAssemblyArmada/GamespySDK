add_library(gschat OBJECT)
target_sources(gschat PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/chat/chat.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/chat/chatascii.h
    chatcallbacks.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chat/chatcallbacks.h
    chatchannel.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chat/chatchannel.h
    chatcrypt.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chat/chatcrypt.h
    chathandlers.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chat/chathandlers.h
    chatmain.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chat/chatmain.h
    chatsocket.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/chat/chatsocket.h
)

target_link_libraries(gschat PUBLIC gsinterface)
set_target_properties(gschat PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
