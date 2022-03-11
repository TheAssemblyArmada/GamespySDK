add_library(gssc OBJECT)
target_sources(gssc PRIVATE
    sc.h
    sci.h
    sciinterface.c
    sciinterface.h
    scimain.c
    scireport.c
    scireport.h
    sciserialize.c
    sciserialize.h
    sciwebservices.c
    sciwebservices.h
)

target_link_libraries(gssc PUBLIC gsinterface)
set_target_properties(gssc PROPERTIES POSITION_INDEPENDENT_CODE ON)
