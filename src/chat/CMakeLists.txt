add_library(gschat OBJECT)
target_sources(gschat PRIVATE
    chat.h
    chatascii.h
    chatcallbacks.c
    chatcallbacks.h
    chatchannel.c
    chatchannel.h
    chatcrypt.c
    chatcrypt.h
    chathandlers.c
    chathandlers.h
    chatmain.c
    chatmain.h
    chatsocket.c
    chatsocket.h
)

target_link_libraries(gschat PUBLIC gsinterface)
set_target_properties(gschat PROPERTIES POSITION_INDEPENDENT_CODE ON)
