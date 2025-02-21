add_library(gsinterface INTERFACE)
find_package(Threads REQUIRED)
target_link_libraries(gsinterface INTERFACE Threads::Threads)
target_compile_definitions(gsinterface INTERFACE ${GS_COMPILE_DEFS} $<$<CONFIG:DEBUG>:GSI_COMMON_DEBUG=1>)
target_compile_options(gsinterface INTERFACE ${GS_C_FLAGS})
target_include_directories(gsinterface INTERFACE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/gamespy
)

# Handle linking sockets library.
if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    if(GS_WINSOCK2)
        target_link_libraries(gsinterface INTERFACE ws2_32)
    else()
        target_link_libraries(gsinterface INTERFACE wsock32)
    endif()
	
	if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16.0")
        target_include_directories(gsinterface INTERFACE stdint)
    endif()
endif()

add_subdirectory(common)
add_subdirectory(chat)
add_subdirectory(natneg)
add_subdirectory(qr2)
add_subdirectory(gcdkey)
add_subdirectory(ghttp)
add_subdirectory(gt2)
add_subdirectory(gp)
add_subdirectory(gstats)
add_subdirectory(pinger)
add_subdirectory(serverbrowsing)
add_subdirectory(peer)
add_subdirectory(pt)
add_subdirectory(sake)
add_subdirectory(webservices)
add_subdirectory(sc)

set(GAMESPY_MODULES
    gscommon
    gschat
    gsnatneg
    gsqr2
    gscdkey
    gshttp
    gsgt2
    gsgp
    gsgstats
    gspinger
    gsserverbrowsing
    gspeer
    gspt
    gssake
    gswebservices
    gssc
)

if(GS_INCLUDE_VOICE)
    add_subdirectory(voice2)
    list(APPEND GAMESPY_MODULES gsvoice2)
endif()

add_library(gamespy)
add_library(gamespy::gamespy ALIAS gamespy)

target_link_libraries(gamespy PRIVATE ${GAMESPY_MODULES})

target_include_directories(gamespy PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/gamespy>
    $<INSTALL_INTERFACE:include>
)

if(BUILD_SHARED_LIBS)
    set_target_properties(gamespy PROPERTIES ARCHIVE_OUTPUT_NAME gamespy_import WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(GS_BUILD_TESTS)
    add_executable(chattest chat/chatc/chatc.c)
    target_link_libraries(chattest gamespy)
    target_compile_definitions(chattest PRIVATE test_main=main)
    
    add_executable(natnegtest natneg/simpletest/simpletest.c)
    target_link_libraries(natnegtest gamespy)
    target_compile_definitions(natnegtest PRIVATE test_main=main)
    
    add_executable(qr2sample qr2/qr2csample/qr2csample.c)
    target_link_libraries(qr2sample gamespy)
    target_compile_definitions(qr2sample PRIVATE test_main=main)
     
    add_executable(keyclienttest gcdkey/clienttest/gcdkeyclienttest.c)
    target_link_libraries(keyclienttest gamespy)
    target_compile_definitions(keyclienttest PRIVATE test_main=main)
    add_executable(keyservertest gcdkey/servertest/gcdkeyservertest.c)
    target_link_libraries(keyservertest gamespy)
    target_compile_definitions(keyservertest PRIVATE test_main=main)
    
    add_executable(httptest ghttp/ghttpc/ghttpc.c)
    target_link_libraries(httptest gamespy)
    target_compile_definitions(httptest PRIVATE test_main=main)
    
    add_executable(gt2test gt2/gt2testc/gt2testc.c)
    target_link_libraries(gt2test gamespy)
    target_compile_definitions(gt2test PRIVATE test_main=main)
    
    add_executable(gptest gp/gptestc/gptestc.c)
    target_link_libraries(gptest gamespy)
    target_compile_definitions(gptest PRIVATE test_main=main)
    
    add_executable(statstest gstats/statstest/statstest.c)
    target_link_libraries(statstest gamespy)
    target_compile_definitions(statstest PRIVATE test_main=main)
    add_executable(persisttest gstats/persisttest/persisttest.c)
    target_link_libraries(persisttest gamespy)
    target_compile_definitions(persisttest PRIVATE test_main=main)
    
    add_executable(sbtest serverbrowsing/sbctest/sbctest.c)
    target_link_libraries(sbtest gamespy)
    target_compile_definitions(sbtest PRIVATE test_main=main)
    
    add_executable(peertest peer/peerc/peerc.c)
    target_link_libraries(peertest gamespy)
    target_compile_definitions(peertest PRIVATE test_main=main)
    
    add_executable(pttest pt/pttestc/pttestc.c)
    target_link_libraries(pttest gamespy)
    target_compile_definitions(pttest PRIVATE test_main=main)
    
    add_executable(saketest sake/saketest/saketest.c)
    target_link_libraries(saketest gamespy)
    target_compile_definitions(saketest PRIVATE test_main=main)
    
    add_executable(sctest sc/sctest/sctest.c)
    target_link_libraries(sctest gamespy)
    target_compile_definitions(sctest PRIVATE test_main=main)
    add_executable(sctestmatchless sc/sctestmatchless/sctestmatchless.c)
    target_link_libraries(sctestmatchless gamespy)
    target_compile_definitions(sctestmatchless PRIVATE test_main=main)
endif()
