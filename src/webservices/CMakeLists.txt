add_library(gswebservices OBJECT)
target_sources(gswebservices PRIVATE
    authservice.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/webservices/authservice.h
)

target_link_libraries(gswebservices PUBLIC gsinterface)
set_target_properties(gswebservices PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
