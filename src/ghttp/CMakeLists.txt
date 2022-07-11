add_library(gshttp OBJECT)
target_sources(gshttp PRIVATE
    ghttp.h
    ghttpascii.h
    ghttpbuffer.c
    ghttpbuffer.h
    ghttpcallbacks.c
    ghttpcallbacks.h
    ghttpcommon.c
    ghttpcommon.h
    ghttpconnection.c
    ghttpconnection.h
    ghttpencryption.c
    ghttpencryption.h
    ghttpmain.c
    ghttpmain.h
    ghttppost.c
    ghttppost.h
    ghttpprocess.c
    ghttpprocess.h
    gssoap.c
    gssoap.h
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
