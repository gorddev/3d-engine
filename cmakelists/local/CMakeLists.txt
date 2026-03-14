# options for compiling locally

# compiler warning

target_link_options(ECSEngine PRIVATE
    -fsanitize=address
    -fsanitize=undefined
)

target_compile_options(ECSEngine PRIVATE
    -Werror
    -Wall
    -DGL_SILENCE_DEPRECATION
    -O1
    -fsanitize=address
    -fsanitize=undefined
    -g
)



if(APPLE)   # other libraries
    message("linking apple libs")
    target_link_libraries(ECSEngine PRIVATE
        Render_Lib
        SDL3::SDL3
        glm::glm
        OpenGL::GL
        glfw
        glbinding::glbinding
    )
else()
    target_link_libraries(ECSEngine PRIVATE
        Render_Lib
        SDL3::SDL3
        glm::glm
        OpenGL::GL
    )
endif()

