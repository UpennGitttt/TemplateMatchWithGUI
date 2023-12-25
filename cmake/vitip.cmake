set(vitip_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/vitip_win)
set(TARGET_NAME vitip)
set(Vitip_LOCAL_PATH ${VITIP_ROOT_DIR})

add_library(vitip3 SHARED IMPORTED)  # 这个必须要有
# set(ViTip_3rd_DIR ${CMAKE_SOURCE_DIR}/3rdparty/vitip)

set_target_properties(vitip3 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${vitip_SOURCE_DIR}/include"
)
set_property(TARGET vitip3 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
# Import target "vitip3" for configuration "Debug"
# set_target_properties(vitip3 PROPERTIES
#     IMPORTED_IMPLIB_DEBUG "${vitip_SOURCE_DIR}/lib/vitip3-debug.lib"
#     IMPORTED_LOCATION_DEBUG "${vitip_SOURCE_DIR}/bin/vitip3-debug.dll"
# )
# Import target "vitip3" for configuration "Release"
set_target_properties(vitip3 PROPERTIES
    IMPORTED_IMPLIB_RELEASE "${vitip_SOURCE_DIR}/lib/vitip3.lib"
    IMPORTED_LOCATION_RELEASE "${vitip_SOURCE_DIR}/bin/vitip3.dll"
)

set(Vitip_INCLUDE_DIRS ${vitip_SOURCE_DIR}/include  CACHE STRING "" FORCE)
file(GLOB  Vitip_LIBRARY "${Vitip_LOCAL_PATH}/lib/*")
file(GLOB Vitip_DLLS "${vitip_SOURCE_DIR}/bin/*.dll")
# file(COPY ${Vitip_DLLS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

#if(EXISTS ${vitip_SOURCE_DIR})
#    set(Vitip_INCLUDE_DIRS ${vitip_SOURCE_DIR}/include  CACHE STRING "" FORCE)
#    file(GLOB  Vitip_LIBRARY "${Vitip_LOCAL_PATH}/lib/*")
#    set(Vitip_LIBRARIES ${Vitip_LIBRARY})

    # 复制 DLL 文件到构建目录
    #file(GLOB Vitip_DLLS "${Vitip_LOCAL_PATH}/bin/*.dll")
    #file(COPY ${Vitip_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
#endif()

set_target_properties(vitip3 PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)
add_library(vitip::vitip3 ALIAS vitip3)
