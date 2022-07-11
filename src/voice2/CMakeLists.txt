add_library(gsvoice2 OBJECT)
target_sources(gsvoice2 PRIVATE
    gvcodec.c
    gvcodec.h
    gvcustomdevice.c
    gvcustomdevice.h
    gvdevice.c
    gvdevice.h
    gvframe.c
    gvframe.h
    gvmain.c
    gvmain.h
    gvsource.c
    gvsource.h
    gvspeex.c
    gvspeex.h
    gvutil.c
    gvutil.h
)

if(WIN32 OR ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_sources(gsvoice2 PRIVATE
        gvdirectsound.c
        gvdirectsound.h
    )
    
    target_link_libraries(gsvoice2 PUBLIC dsound dxguid)
elseif(APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_sources(gsvoice2 PRIVATE
        gvosxaudio.c
        gvosxaudio.h
    )
    target_link_libraries(gsvoice2 PUBLIC "-framework CoreAudio" "-framework AudioToolbox" "-framework CoreFoundation")
endif()

if(UNIX AND NOT APPLE)
    target_compile_definitions(gsvoice2 PRIVATE -DGV_NO_DEFAULT_HARDWARE)
endif()

target_link_libraries(gsvoice2 PUBLIC gsinterface)
set_target_properties(gsvoice2 PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})

target_link_libraries(gsvoice2 PUBLIC ${SPEEX_LIBRARIES})
target_include_directories(gsvoice2 PRIVATE $<BUILD_INTERFACE:${SPEEX_INCLUDE_DIRS}>)
