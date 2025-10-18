cmake_minimum_required(VERSION 3.10)
project(gltest)
# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE RELEASE)

cmake_policy(SET CMP0072 NEW)

#find_package(glfw3 REQUIRED)
#find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(SDL3)

set(GLTEST_DIR "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GLTEST_DIR}/$<CONFIGURATION>")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GLTEST_DIR}/$<CONFIGURATION>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GLTEST_DIR}/$<CONFIGURATION>")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GLTEST_DIR}/$<CONFIGURATION>")

add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)

set(SDL_SHARED OFF CACHE BOOL "Build shared SDL library")
set(SDL_STATIC ON CACHE BOOL "Build static SDL library")
add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)
add_subdirectory(third_party/stb)

#set SOURCES to all cpp files in src
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp"
                  "${PROJECT_SOURCE_DIR}/src/gl_utils/*.cpp"
                  "${PROJECT_SOURCE_DIR}/src/sdl_utils/*.cpp"
                  "${PROJECT_SOURCE_DIR}/src/data_structures/*.cpp")


add_executable(test "${PROJECT_SOURCE_DIR}/src/main.cpp" ${SOURCES})
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(test PRIVATE -Wall -Wextra -Werror)# -Wconversion -Wpedantic)
endif()

target_link_libraries(test PRIVATE glad)
#target_link_libraries(test OPENGL::GL)

target_link_libraries(test PRIVATE SDL3::SDL3-static)
target_link_libraries(test PRIVATE stb)

target_include_directories(test
        PUBLIC ${PROJECT_SOURCE_DIR}/include/
        PRIVATE ${PROJECT_SOURCE_DIR}/src
)

set_target_properties(test PROPERTIES
    OUTPUT_NAME "run"
)

set_target_properties( PROPERTIES VERSION 1.0 SOVERSION 1)


#install
#install(TARGETS test
#	RUNTIME DESTINATION ${GLTEST_DIR}
#)
