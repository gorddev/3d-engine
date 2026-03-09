# options for compiling locally

# compiler warning
if (APPLE)
target_compile_options(ECSEngine PRIVATE
        -Werror
        -Wall
        -DGL_SILENCE_DEPRECATION
        -O3
)
elseif(UNIX)
target_compile_options(ECSEngine PRIVATE
    -Werror
    -Wall
    -DGL_SILENCE_DEPRECATION
    -O3
    -DSDL_X11_XTEST=OFF
)
endif()



if(APPLE)
# other libraries
target_link_libraries(ECSEngine PRIVATE
        Render_Lib
        SDL3::SDL3
        glm::glm
        OpenGL::GL
        glfw
)
else()
    target_link_libraries(ECSEngine PRIVATE
        Render_Lib
        SDL3::SDL3
        glm::glm
        OpenGL::GL
    )
endif()


if(APPLE)
    target_link_libraries(ECSEngine PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
endif()