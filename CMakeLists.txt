cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 23)

project(WebEngine)

#find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)



if(UNIX AND NOT APPLE AND NOT EMSCRIPTEN)
    message("Building for Linux")
    set(X11_DEPS_COMMAND "Please use the build dependencies listed here:\nhttps://wiki.libsdl.org/SDL3/README-linux#build-dependencies\n")

    find_package(X11 REQUIRED)
    find_path(XRANDR_PATH NAMES X11/extensions/Xrandr.h)

    if(NOT XRANDR_PATH)
        message(FATAL_ERROR
            " Missing Dependencies to compile the engine:\n"
            " \tGLFW requires X11 development headers to build from source.\n"
            " \tPlease run the following command to install them:\n"
            "   ${X11_DEPS_COMMAND}\n"
        )
    endif()
elseif(APPLE)
    message("Building for Apple")
elseif(EMSCRIPTEN)
    message("Building for Emscripten")
endif()

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
elseif(APPLE)
    # otherwise we need to get it.
    FetchContent_Declare(
            GLFW
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            DIRECTORY include/GLFW
            GIT_TAG master
    )
    # set a bunch of things so we don't get error when building
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(GLFW)
endif()

# now we need sdl3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
    CMAKE_ARGS
        -DSDL_X11_XINPUT=OFF
        -DSDL_X11_XINPUT2=OFF
        -DSDL_X11_XSCRNSAVER=OFF
        -DSDL_X11_XTEST=OFF
        -DSDL_X11_XFIXES=OFF
        -DSDL_WAYLAND=OFF
        -DSDL_RENDER=OFF
        -DSDL_DBUS=OFF
        -DSDL_IBUS=OFF
        -DSDL_CAMERA=OFF
        -DSDL_TEST_LIBRARY=OFF
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