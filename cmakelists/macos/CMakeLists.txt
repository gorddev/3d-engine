# cmakelists for running on Macos

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# Find glfw for macos
FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    DIRECTORY include/GLFW
    GIT_TAG master
)
# set a bunch of things so we don't get error when building
FetchContent_MakeAvailable(GLFW)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_Declare(
    glbinding
    GIT_REPOSITORY https://github.com/cginternals/glbinding.git
    GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(glbinding)

