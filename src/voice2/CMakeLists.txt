add_library(gsvoice2 OBJECT)
target_sources(gsvoice2 PRIVATE
    gvcodec.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gvcodec.h
    gvcustomdevice.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gvcustomdevice.h
    gvdevice.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gvdevice.h
    gvframe.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gvframe.h
    gvmain.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gvmain.h
    gvsource.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gvsource.h
    gvspeex.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gvspeex.h
    gvutil.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gvutil.h
)

if(WIN32 OR ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_sources(gsvoice2 PRIVATE
        gvdirectsound.c
        ${PROJECT_SOURCE_DIR}/include/gamespy/gvdirectsound.h
    )
    
    target_link_libraries(gsvoice2 PUBLIC dsound dxguid)
elseif(APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_sources(gsvoice2 PRIVATE
        gvosxaudio.c
        ${PROJECT_SOURCE_DIR}/include/gamespy/gvosxaudio.h
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
