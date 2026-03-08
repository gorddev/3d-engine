cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 23)

project(WebEngine)

include(FetchContent)
############ GLM
# GLM Fetch
FetchContent_Declare(
        GLM
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        DIRECTORY glm
        GIT_TAG master
)
FetchContent_MakeAvailable(GLM) # makes content available
# GLFW fetch
FetchContent_Declare(
        GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        DIRECTORY include/GLFW
        GIT_TAG master
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(GLFW)

add_subdirectory(src)

add_executable(ECSEngine
        main.cpp
)

option(WEB_VERSION "Enable to render to web." OFF)

if(WEB_VERSION)
    set(IMPL_DIR "emscripten")
else()
    set(IMPL_DIR "local")
endif()

add_subdirectory("${IMPL_DIR}")