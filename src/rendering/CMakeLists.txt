

set(RENDERING_SRC
        Renderer.cpp
        Window.cpp
        glFunctions/glTextures.cpp
        glFunctions/glShaders.cpp
        glFunctions/glBuffers.cpp
        shaders/Shader.cpp
        shaders/ShaderManager.cpp
        texture/stb_image.c
        texture/TextureRegister.cpp
)

add_library(Render_Lib STATIC ${RENDERING_SRC})

# Specify include directories for consumers of the library
target_include_directories(Render_Lib PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external
)

if (APPLE)
target_link_libraries(Render_Lib PRIVATE
        SDL3::SDL3
        glm::glm
        glfw
)
else()
target_link_libraries(Render_Lib PRIVATE
    SDL3::SDL3
    glm::glm
)
endif()
