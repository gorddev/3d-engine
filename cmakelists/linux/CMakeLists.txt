
set(X11_DEPS_COMMAND "Please use the build dependencies listed here:\nhttps://wiki.libsdl.org/SDL3/README-linux#build-dependencies\n")

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

if(NOT XRANDR_PATH)
    message(FATAL_ERROR
        " Missing Dependencies to compile the engine:\n"
        " \tGLFW requires X11 development headers to build from source.\n"
        " \tPlease go to the following website to install them:\n"
        "   ${X11_DEPS_COMMAND}\n"
    )
endif()