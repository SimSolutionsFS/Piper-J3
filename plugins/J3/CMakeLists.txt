cmake_minimum_required(VERSION 3.24)
project(SimSolutionsJ3 VERSION 1.0.2 DESCRIPTION "J3 for X-Plane 11")

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# CMake Settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC -static-libgcc -shared")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fPIC -static-libgcc -shared")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/64")

# Definitions for the X-Plane SDK
add_definitions(
	-DXPLM200
	-DXPLM210
	-DXPLM300
	-DXPLM301
	-DXPLM302
	-DXPLM303
)

# Files to compile
add_executable(SimSolutionsJ3
	src/plugin.c
	src/misc.c
	src/radio.cpp
)
set_target_properties(SimSolutionsJ3 PROPERTIES SUFFIX ".xpl")

# Include dependencies
include_directories(SimSolutionsJ3
	"libs/XPSDK/CHeaders/XPLM"
	"libs/XPSDK/CHeaders/Widgets"
	"libs/libacfutils/include"
	"libs/stb"
)

if (WIN32)
	set(ARCH "mingw64")
elseif (APPLE)
	set(ARCH "mac64")
else ()
	set(ARCH "lin64")
endif ()

# Adapted from https://discord.com/channels/397379810067742721/529085059324444672/1029105813631029308
set(LACF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/${ARCH}/lib")
set(LACF_LIBS
	"${LACF_ROOT}/libacfutils.a"
	"${LACF_ROOT}/libiconv.a"
	"${LACF_ROOT}/libcairo.a"
	"${LACF_ROOT}/libpixman-1.a"
	"${LACF_ROOT}/libfreetype.a"
	"${LACF_ROOT}/libopusurl.a"
	"${LACF_ROOT}/libopusfile.a"
	"${LACF_ROOT}/libopus.a"
	"${LACF_ROOT}/libogg.a"
	"${LACF_ROOT}/libpcre2-8.a"
	"${LACF_ROOT}/libproj.a"
	"${LACF_ROOT}/libxml2.a"
	"${LACF_ROOT}/libcurl.a"
	"${LACF_ROOT}/libshp.a"
	"${LACF_ROOT}/libcharset.a"
	"${LACF_ROOT}/libpng16.a"
	"${LACF_ROOT}/liblzma.a"
	"${LACF_ROOT}/libz.a"
	"${LACF_ROOT}/libssl.a"
	"${LACF_ROOT}/libcrypto.a"
)

if (WIN32)
	list(APPEND LACF_LIBS "${LACF_ROOT}/libglew32mx.a" crypt32 ws2_32 gdi32 dbghelp psapi winmm)
else ()
	list(APPEND LACF_LIBS "${LACF_ROOT}/libGLEWmx.a" pthread)
endif ()

# OS-specific options
if (WIN32)
	set_target_properties(SimSolutionsJ3 PROPERTIES OUTPUT_NAME "win")
	include_directories("libs/libacfutils/mingw64/include")
	add_definitions(
		-DIBM
		-DGLEW_STATIC
		-D_USE_MATH_DEFINES
	)
	target_link_libraries(SimSolutionsJ3
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/XPSDK/Libraries/Win/XPLM_64.lib"
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/XPSDK/Libraries/Win/XPWidgets_64.lib"
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/libacfutils/win64/lib/libacfutils37.a"
		opengl32
		imagehlp
		Psapi
		stdc++
	)
elseif (APPLE)
	set_target_properties(SimSolutionsJ3 PROPERTIES OUTPUT_NAME "mac")
	include_directories("libs/libacfutils/mac64/include")
	add_definitions(
		-DAPL
		-DGL_SILENCE_DEPRECATION
	)
	target_link_libraries(SimSolutionsJ3
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/XPSDK/Libraries/Mac/XPLM.framework"
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/XPSDK/Libraries/Mac/XPWidgets.framework"
		${LACF_LIBS}
		"-framework CoreFoundation"
		stdc++
	)
else ()
	set_target_properties(SimSolutionsJ3 PROPERTIES OUTPUT_NAME "lin")
	include_directories("libs/libacfutils/lin64/include")
	add_definitions(-DLIN)
	target_link_libraries(SimSolutionsJ3
		${LACF_LIBS}
		stdc++
	)
endif ()
