cmake_minimum_required(VERSION 3.24)
project(Piper-J3 VERSION 1.0.3)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# CMake Settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC -shared")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fPIC -shared")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

if (WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/win_x64")
elseif (APPLE)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/mac_x64")
else ()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lin_x64")
endif ()

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
add_executable(Piper-J3
	src/plugin.c
	src/misc.c
	src/radio.cpp
)
set_target_properties(Piper-J3 PROPERTIES OUTPUT_NAME "J3")
set_target_properties(Piper-J3 PROPERTIES SUFFIX ".xpl")

# Include dependencies
add_subdirectory("simcore")
add_dependencies(Piper-J3 simcore)

# OS-specific options
if (WIN32)
	set(CMAKE_CXX_FLAGS "-O2 -fPIC -static-libgcc -static -shared")
	set(CMAKE_C_FLAGS "-O2 -fPIC -static-libgcc -static -shared")
	add_definitions(
		-DIBM
		-DGLEW_STATIC
		-D_USE_MATH_DEFINES
	)
	target_link_libraries(Piper-J3
		simcore
		opengl32
		imagehlp
		Psapi
		stdc++
	)
elseif (APPLE)
	add_definitions(
		-DAPL
		-DGL_SILENCE_DEPRECATION
	)
	target_link_libraries(Piper-J3
		simcore
		"-framework CoreFoundation"
		stdc++
	)
else ()
	add_definitions(-DLIN)
	target_link_libraries(Piper-J3
		simcore
		stdc++
	)
endif ()