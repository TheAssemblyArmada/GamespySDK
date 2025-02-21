add_library(gshttp OBJECT)
target_sources(gshttp PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttp.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttpascii.h
    ghttpbuffer.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttpbuffer.h
    ghttpcallbacks.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttpcallbacks.h
    ghttpcommon.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttpcommon.h
    ghttpconnection.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttpconnection.h
    ghttpencryption.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttpencryption.h
    ghttpmain.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttpmain.h
    ghttppost.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttppost.h
    ghttpprocess.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/ghttpprocess.h
    gssoap.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gssoap.h
)

target_link_libraries(gshttp PUBLIC gsinterface)
set_target_properties(gshttp PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
target_compile_definitions(gshttp PUBLIC $<$<CONFIG:DEBUG>:GHTTP_EXTENDEDERROR=1>)

if(GS_HTTPLOG)
	target_compile_definitions(gshttp PUBLIC HTTP_LOG=1)
endif()

if(GS_OPENSSL)
    target_link_libraries(gshttp PUBLIC OpenSSL::SSL)
    target_compile_definitions(gshttp PRIVATE OPENSSL=1)
endif()
