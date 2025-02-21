add_library(gscommon OBJECT)
target_sources(gscommon PRIVATE
    darray.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/darray.h
    gsassert.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsassert.h
    gsavailable.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsavailable.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/gscommon.h
    gscore.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gscore.h
    gscrypt.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gscrypt.h
    gsdebug.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsdebug.h
    gslargeint.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gslargeint.h
    gsmemory.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsmemory.h
    gsplatform.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsplatform.h
    gsplatformsocket.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsplatformsocket.h
    gsplatformthread.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsplatformthread.h
    gsplatformutil.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsplatformutil.h
    gsrc4.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsrc4.h
    gssha1.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gssha1.h
    gsssl.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsssl.h
    gsstringutil.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsstringutil.h
    gsxml.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/gsxml.h
    hashtable.c
    ${PROJECT_SOURCE_DIR}/include/gamespy/hashtable.h
    ${PROJECT_SOURCE_DIR}/include/gamespy/md5.h
    md5c.c
)

target_link_libraries(gscommon PUBLIC gsinterface)
set_target_properties(gscommon PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
