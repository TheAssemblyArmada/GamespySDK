add_library(gscommon OBJECT)
target_sources(gscommon PRIVATE
    darray.c
    darray.h
    gsassert.c
    gsassert.h
    gsavailable.c
    gsavailable.h
    gscommon.h
    gscore.c
    gscore.h
    gscrypt.c
    gscrypt.h
    gsdebug.c
    gsdebug.h
    gslargeint.c
    gslargeint.h
    gsmemory.c
    gsmemory.h
    gsplatform.c
    gsplatform.h
    gsplatformsocket.c
    gsplatformsocket.h
    gsplatformthread.c
    gsplatformthread.h
    gsplatformutil.c
    gsplatformutil.h
    gsrc4.c
    gsrc4.h
    gssha1.c
    gssha1.h
    gsssl.c
    gsssl.h
    gsstringutil.c
    gsstringutil.h
    gsxml.c
    gsxml.h
    hashtable.c
    hashtable.h
    md5.h
    md5c.c
)

target_link_libraries(gscommon PUBLIC gsinterface)
set_target_properties(gscommon PROPERTIES POSITION_INDEPENDENT_CODE ON)
