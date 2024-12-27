cmake_minimum_required(VERSION 3.24)
project(SimSolutionsJ3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2 -fPIC -shared")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 64)

add_executable(SimSolutionsJ3 
    src/plugin.cpp
    src/misc.cpp
    src/radio.cpp
)

set_target_properties(SimSolutionsJ3 PROPERTIES SUFFIX ".xpl")

add_definitions(
    -DXPLM200
    -DXPLM210
    -DXPLM300
    -DXPLM301
    -DXPLM302
    -DXPLM303
)

include_directories(
    "libs/XPSDK/CHeaders/XPLM"
    "libs/XPSDK/CHeaders/Widgets"
    "libs/libacfutils/include"
    "libs/stb"
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set_target_properties(SimSolutionsJ3 PROPERTIES OUTPUT_NAME "win")
    include_directories("libs/libacfutils/win64/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc")
    add_definitions(
        -DIBM
        -DGLEW_STATIC
        -D_USE_MATH_DEFINES
    )
    target_link_libraries(SimSolutionsJ3
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/XPSDK/Libraries/Win/XPLM_64.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/XPSDK/Libraries/Win/XPWidgets_64.lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/win64/lib/libacfutils32.a"
        opengl32
        imagehlp
        Psapi
		stdc++
    )
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set_target_properties(SimSolutionsJ3 PROPERTIES OUTPUT_NAME "mac")
    include_directories("libs/libacfutils/mac64/include")
    add_definitions(-DAPL)
    target_link_libraries(SimSolutionsJ3
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libGLEWmx.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libacfutils.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libfreetype.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libcairo.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/mac64/lib/libz.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/XPSDK/Libraries/Mac/XPLM.framework"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/XPSDK/Libraries/Mac/XPWidgets.framework"
        "-framework OpenGL"
        "-framework CoreFoundation"
        stdc++
    )
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set_target_properties(SimSolutionsJ3 PROPERTIES OUTPUT_NAME "lin")
    include_directories("libs/libacfutils/lin64/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    add_definitions(-DLIN)
    target_link_libraries(SimSolutionsJ3
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/lin64/lib/libGLEWmx.a"
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/lin64/lib/libacfutils.a"
        stdc++
    )
endif ()