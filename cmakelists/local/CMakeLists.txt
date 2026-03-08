# options for compiling locally

# compiler warning
target_compile_options(ECSEngine PRIVATE
        -Werror
        -Wall
        -DGL_SILENCE_DEPRECATION
        -O3
)

# other libraries
target_link_libraries(ECSEngine PRIVATE
        Render_Lib
        SDL3::SDL3
        glm::glm
        OpenGL::GL
        glfw
)

if(APPLE)
    target_link_libraries(ECSEngine PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
endif()