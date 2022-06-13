cmake_minimum_required(VERSION 3.9.1)
project(SimSolutionsJ3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -fPIC -fvisibility=hidden -Wno-narrowing")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 64)

add_executable(SimSolutionsJ3 
    src/plugin.cpp
    src/misc.cpp
    src/radio.cpp
)

set_target_properties(SimSolutionsJ3 PROPERTIES SUFFIX ".xpl")

add_definitions(-DXPLM200)
add_definitions(-DXPLM210)
add_definitions(-DXPLM300)
add_definitions(-DXPLM301)
add_definitions(-DXPLM302)
add_definitions(-DXPLM303)

include_directories("libs/XSDK/CHeaders/XPLM")
include_directories("libs/XSDK/CHeaders/Widgets")
include_directories("libs/libacfutils/include")
include_directories("libs/stb")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set_target_properties(SimSolutionsJ3 PROPERTIES OUTPUT_NAME "win")

    include_directories("libs/libacfutils/win64/include")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static -shared -Wl,--version-script=exports.txt")
    add_definitions(-DIBM)
    add_definitions(-DGLEW_STATIC)
    add_definitions(-D_USE_MATH_DEFINES)

    target_link_libraries(SimSolutionsJ3
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/XSDK/Libraries/Win/XPLM_64.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/XSDK/Libraries/Win/XPWidgets_64.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/win64/lib/libacfutils32.a"
        opengl32
        imagehlp
        Psapi
		stdc++
    )
endif (CMAKE_SYSTEM_NAME MATCHES "Windows")

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set_target_properties(SimSolutionsJ3 PROPERTIES OUTPUT_NAME "mac")

    include_directories("libs/libacfutils/mac64/include")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -Wl,-exported_symbol -Wl,_XPluginStart -Wl,-exported_symbol -Wl,_XPluginEnable -Wl,-exported_symbol -Wl,_XPluginReceiveMessage -Wl,-exported_symbol -Wl,_XPluginDisable -Wl,-exported_symbol -Wl,_XPluginStop")
    add_definitions(-DAPL)

    target_link_libraries(SimSolutionsJ3
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libGLEWmx.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libacfutils.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libfreetype.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libcairo.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libz.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/XSDK/Libraries/Mac/XPLM.framework"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/XSDK/Libraries/Mac/XPWidgets.framework"
        "-framework OpenGL"
        "-framework CoreFoundation"
        stdc++
    )
endif (CMAKE_SYSTEM_NAME MATCHES "Darwin")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set_target_properties(SimSolutionsJ3 PROPERTIES OUTPUT_NAME "lin")

    include_directories("libs/libacfutils/lin64/include")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -shared -Wl,--version-script=exports.txt")
    add_definitions(-DLIN)

    target_link_libraries(SimSolutionsJ3
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/lin64/lib/libGLEWmx.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/lin64/lib/libacfutils.a"
        stdc++
    )
endif (CMAKE_SYSTEM_NAME MATCHES "Linux")