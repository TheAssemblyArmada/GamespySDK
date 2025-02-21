add_library(gssc OBJECT)
target_sources(gssc PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/sc.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/sci.h
    sciinterface.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sciinterface.h
    scimain.c
    scireport.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/scireport.h
    sciserialize.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sciserialize.h
    sciwebservices.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sciwebservices.h
)

target_link_libraries(gssc PUBLIC gsinterface)
set_target_properties(gssc PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
