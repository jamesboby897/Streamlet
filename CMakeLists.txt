cmake_minimum_required(VERSION 3.22.1)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(Viewer
	DESCRIPTION "Simple LVF Viewer"
	LANGUAGES C
)


add_executable(${PROJECT_NAME} viewer.c)
add_executable(lvfenc lvfenc.c)
add_executable(lvfdec lvfdec.c)


add_subdirectory(libs/glfw)
add_subdirectory(libs/gl3w)
add_subdirectory(libs/opus)

target_include_directories(lvfenc
	PUBLIC libs/qoi
	PUBLIC libs/opus/include
)

target_include_directories(lvfdec
	PUBLIC libs/qoi
	PUBLIC libs/opus/include
)

target_include_directories(${PROJECT_NAME}
	PUBLIC libs/glfw/include
	PUBLIC libs/gl3w/include
	PUBLIC libs/opus/include
	PUBLIC libs/qoi/
	PUBLIC libs/miniaudio
)

target_link_directories(${PROJECT_NAME}
	PUBLIC libs/glfw/src
	PUBLIC libs/gl3w/src
	PUBLIC libs/opus/src
)

target_link_directories(lvfenc
	PUBLIC 	PUBLIC libs/opus/include
)

target_link_directories(lvfdec
	PUBLIC 	PUBLIC libs/opus/include
)

target_link_libraries(${PROJECT_NAME}
	glfw
	gl3w
	opus
)

target_link_libraries(lvfenc
	opus
)

target_link_libraries(lvfdec
	opus
)