cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 23)

project(WebEngine)
include(FetchContent)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)



if(UNIX AND NOT APPLE AND NOT EMSCRIPTEN)
    find_package(OpenGL REQUIRED)
    message("Building for Linux")
    add_subdirectory("cmakelists/linux")
elseif(APPLE)
    message("Building for MacOS")
    add_subdirectory("cmakelists/macos")
elseif(EMSCRIPTEN)
    message("Building for Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
else()
    message("Error: Invalid Build Platform")
endif()

############ GLM
# GLM Fetch
FetchContent_Declare(
        GLM
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        DIRECTORY glm
        GIT_TAG master
)
FetchContent_MakeAvailable(GLM) # makes content available


# find all the .cpp files
add_subdirectory(src)

# add just our main.cpp
add_executable(ECSEngine
        main.cpp
        src/olaf-api/EventDispatcher.cpp
        src/Engine.cpp
)

# our include directories: we want just include & external
target_include_directories(ECSEngine PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external
)

if(EMSCRIPTEN)
    set(IMPL_DIR "cmakelists/emscripten")
else()
    set(IMPL_DIR "cmakelists/local")
endif()

add_subdirectory("${IMPL_DIR}")