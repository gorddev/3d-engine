#pragma once

#include <types/enums/RenderingSettings.hpp>
#include <rendering/Window.hpp>
#include "../scene/Camera.hpp"
#include "RenderQueue.hpp"
#include "shaders/ShaderManager.hpp"

// made by gordie novak on feb 16th

// new renderer that behaves nicely


namespace gan {

    class ECSCore; //< forward declaration for ECS Core
    class EngineCore; //< forward declaration for EngineCore
    class LayerList; //< forward declaration for LayerList
    class Font; //< forward declaration for Font
    class Model; //< A model the renderer needs in order to draw.

    /// The renderer's job is to render everything assigned to each layer.
    class Renderer {

        /// Reference to the window
        EngineCore& core;
        /// The opengl context handle
        SDL_GLContext gl_context;

        RenderQueue r_queue;        ///< The queue to render objects
    public:
        ShaderHandler shaders;      ///< Stores & updates the uniforms of all the shaders.

        /// The renderer is default constructed.
        explicit Renderer(EngineCore& core);

        ~Renderer(); ///< Destructor

        /// Performs the rendering procedure
        void render();

        /// Draws the given model to the screen with the given camera.
        void draw3D(Model model);

        void clear(Camera &camera);

        /// Places the stuff the renderer has drawn to the screen for the user to see.
        /// Nothing will display unless you call present.
        void present();

        /// Rasterize a font
        void rasterizeFont(Font& font, const char path[], ScaleMode scale_mode);
    private:
        /// Updates the underlying canvas size
        void updateCanvasSize();
        /// Rasterize any textures that needs to be rendered
        void rasterizePendingTextures();
        /// Wipes a texture clean and sets it as the render target.
        /// @param texture The texture you want to wipe and select.
        void setRenderTarget(SDL_Texture* texture);

        static void drawInstance(std::vector<RenderQueue::ModelInstance>& instances, const Shader& shader);
    };


}
