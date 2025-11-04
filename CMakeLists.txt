cmake_minimum_required(VERSION 3.10)
project(gltest)
# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG")
set(CMAKE_BUILD_TYPE Release)

cmake_policy(SET CMP0072 NEW)

#find_package(glfw3 REQUIRED)
#find_package(GLEW REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(SDL3)

set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GLTEST_DIR}/$<CONFIGURATION>")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GLTEST_DIR}/$<CONFIGURATION>")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${GLTEST_DIR}/$<CONFIGURATION>")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${GLTEST_DIR}/$<CONFIGURATION>")

add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)

set(SDL_SHARED OFF CACHE BOOL "Build shared SDL library")
set(SDL_STATIC ON CACHE BOOL "Build static SDL library")
add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)
add_subdirectory(third_party/stb)

#set SOURCES to all cpp files in src
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/gl_utils/*.cpp"
                  "${PROJECT_SOURCE_DIR}/src/gl_utils/renderer/*.cpp"
                  "${PROJECT_SOURCE_DIR}/src/gl_utils/passes/*.cpp"
                  "${PROJECT_SOURCE_DIR}/src/sdl_utils/*.cpp"
                  "${PROJECT_SOURCE_DIR}/src/thread_pool/*.cpp"
                  "${PROJECT_SOURCE_DIR}/src/logger/*.cpp"
                  "${PROJECT_SOURCE_DIR}/src/data_structures/*.cpp")

add_library(engproj_core ${SOURCES})

target_include_directories(engproj_core
        PUBLIC ${PROJECT_SOURCE_DIR}/include/
        PRIVATE ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(engproj_core PUBLIC glad)
target_link_libraries(engproj_core PUBLIC SDL3::SDL3-static)
target_link_libraries(engproj_core PUBLIC stb)

add_executable(engproj "${PROJECT_SOURCE_DIR}/src/main.cpp")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wall -Wextra -Werror)# -Wconversion -Wpedantic)
endif()

#target_link_libraries(engproj PRIVATE glad)
#target_link_libraries(engproj PRIVATE SDL3::SDL3-static)
#target_link_libraries(engproj PRIVATE stb)

#target_include_directories(engproj
#        PUBLIC ${PROJECT_SOURCE_DIR}/include/
#        PRIVATE ${PROJECT_SOURCE_DIR}/src
#)
target_link_libraries(engproj PRIVATE engproj_core)

set_target_properties(engproj PROPERTIES
    OUTPUT_NAME "run" RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/$<CONFIGURATION>"
)

set_target_properties(engproj PROPERTIES VERSION 1.0 SOVERSION 1)


#tests
add_subdirectory(tests)


#install
#install(TARGETS test
#	RUNTIME DESTINATION ${GLTEST_DIR}
#)
