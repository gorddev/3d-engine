cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 23)

project(WebEngine)

#find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

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
if(EMSCRIPTEN)
    # emscripten doesn't need to fetch, as it's a compiler flag
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
else()
    # otherwise we need to get it.
    FetchContent_Declare(
            GLFW
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            DIRECTORY include/GLFW
            GIT_TAG master
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(GLFW)
endif()

# now we need sdl3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
)
FetchContent_MakeAvailable(SDL3)

add_subdirectory(src)

add_executable(ECSEngine
        main.cpp
        src/olaf-api/EventDispatcher.cpp
        src/Engine.cpp
)

target_include_directories(ECSEngine PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external
)

option(WEB_VERSION "Enable to render to web." ON)

if(EMSCRIPTEN)
    set(IMPL_DIR "cmakelists/emscripten")
else()
    set(IMPL_DIR "cmakelists/local")
endif()

add_subdirectory("${IMPL_DIR}")