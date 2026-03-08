#options for compiling to emscripten.

# compiler warning
target_link_options(ECSEngine PRIVATE
        -Werror
        -Wall
        -DGL_SILENCE_DEPRECATION
        "--shell-file=${CMAKE_SOURCE_DIR}/assets/game_shell.html"
        "-O3"
        "-sUSE_WEBGL2=1"
        "-std=c++23"
        #"--preload-file=assets"
        "--emrun"
        "-sUSE_GLFW=3"
        "--browser=chrome"
        "-Wno-error=limited-postlink-optimizations"
        "-sMAXIMUM_MEMORY=512MB"
        "-sALLOW_MEMORY_GROWTH"
        "-DGLFW_BUILD_WAYLAN"
        #"-fwasm-exceptions"

        #"-fsanitize=address"
        #"-fno-omit-frame-pointer"
        #"-sEXPORTED_FUNCTIONS=['_main','_SDL_main']"
        "-sGL_ASSERTIONS=1"
        "-sGL_DEBUG=1"
        "-sASSERTIONS=1"
)

target_link_libraries(ECSEngine PRIVATE
        Render_Lib
        SDL3::SDL3-static
        glm::glm
        #SDL3_image::SDL3_image-static
)

set(CMAKE_TOOLCHAIN_FILE "/Users/gordie/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")


