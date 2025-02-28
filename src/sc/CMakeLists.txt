add_library(gssc OBJECT)
target_sources(gssc PRIVATE
    ${PROJECT_SOURCE_DIR}/include/gamespy/sc/sc.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/sc/sci.h
    sciinterface.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sc/sciinterface.h
    scimain.c
    scireport.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sc/scireport.h
    sciserialize.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sc/sciserialize.h
    sciwebservices.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/sc/sciwebservices.h
)

target_link_libraries(gssc PUBLIC gsinterface)
set_target_properties(gssc PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
