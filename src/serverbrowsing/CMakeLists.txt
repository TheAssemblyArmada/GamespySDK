add_library(gsserverbrowsing OBJECT)
target_sources(gsserverbrowsing PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/sb_ascii.h
    sb_crypt.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sb_crypt.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/sb_internal.h
    sb_queryengine.c
    sb_server.c
    sb_serverbrowsing.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sb_serverbrowsing.h
    sb_serverlist.c
)

target_link_libraries(gsserverbrowsing PUBLIC gsinterface)
set_target_properties(gsserverbrowsing PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
