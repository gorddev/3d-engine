

// created by gordie feb 16th
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "rendering/Renderer.hpp"
#include <OpenGL.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/render-primitives/Model.hpp>

#include "beginner-api/factory/shapes/Colors.hpp"
#include "core/EngineCore.hpp"
#include "glm/gtx/string_cast.hpp"
#include "rendering/render-primitives/Vertex.hpp"

using namespace gan;


Renderer::Renderer(EngineCore& core) : core(core) {

    gl_context = SDL_GL_CreateContext(core.window.sdl_window);

    SDL_GL_MakeCurrent(core.window.sdl_window, gl_context);
    SDL_GL_SetSwapInterval(1);

    printf("GL version: %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    glViewport(0, 0,
        core.window.getDimensions().w,
        core.window.getDimensions().h);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaders.init();
}

Renderer::~Renderer() {
    SDL_GL_DestroyContext(gl_context);
}

struct Renderable;

/*
void Renderer::render() {

    // filter through our ecs stuffs
    //auto filter = ecs.filter<Renderable>();
    // create a double vector of vertices

    glClearColor(0.1f, 0.1f, 0.2f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    //SDL_GL_SwapWindow(window.SDL_GetWindow());
}*/


void Renderer::draw3D(Model model) {
    // add models to the model queue
    r_queue.addModel(model);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Renderer::clear(Camera& camera)  {
    // 1. Clear everything
    glClearColor(0.1f, 0.1f, 0.1f, 0.3f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaders.updateFrameUniforms(core, camera);
}

void Renderer::present()  {
    int count = 0;

    for (auto& [program, instances] : r_queue.modelTree) {
        if (instances.empty())
            continue;

        const auto& shader = *err::unwrap(
            shaders.getShaderByProgramID(program)
            , "Renderer::present()", "Shader requested with program id",
            program, " is invalid as reported by the ShaderHandler.");

        drawInstance(instances, shader);

    }
    // clear our current render queue.
    r_queue.clearModelTree();
    // swap to the current window
    SDL_GL_SwapWindow(core.window.sdl_window);
}

void Renderer::drawInstance(std::vector<RenderQueue::ModelInstance>& instances, const Shader& shader) const {
    // go through each of our instances

    auto current_tex = core.texRg[0].gl_id;
    glUseProgram(shader.getGLProgram());

    for (auto& [vb, mesh, models, colors] : instances) {

        // **************
        // Textures
        // **************
        if (shader.requiresTexture() && current_tex != mesh.tex_id) {
            current_tex = mesh.tex_id;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, core.texRg[0].gl_id);
            glUniform1i(shader.getTexLocation(), 0);
        }

        // **************
        // Colors
        // **************
        if (!vb.cbo) {
            err::panic_if(
                VertexBuffer::genColorVBO(vb, 3),
                "Renderer::drawInstance()",
                "Attempted to create a color buffer object with vao: ", vb.vao, "in file ", __FILE_NAME__
            );
        }
        glBindBuffer(GL_ARRAY_BUFFER, vb.cbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(RGBAVal) * colors.size(),
            colors.data(),
            GL_DYNAMIC_DRAW
        );

        GL_CHECK();

        // **************
        // Model Matrices
        // **************
        if (!shader.requiresModelMatrix()) {
            // If our mbo isn't set up for this shape instance yet, we need to create it.
            if (vb.mbo == 0) {
                err::panic_if(
                    VertexBuffer::genMatrixVBO(vb, 4),
                    "Renderer::drawInstance()",
                    "Attempted to create a matrix buffer object "
                    "for an instance w/vao: ", vb.vao, "in file ", __FILE_NAME__
                );
            }
            // now we create a map to write to
            glBindBuffer(GL_ARRAY_BUFFER, vb.mbo);
            //TODO: Try keeping track of current count of models so we can potentially use glBufferSubData
            glBufferData(
                GL_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(sizeof(glm::mat4) * models.size()),
                models.data(),
                GL_DYNAMIC_DRAW
            );
        }
        glBindVertexArray(vb.vao);
        glDrawArraysInstanced(
            GL_TRIANGLES,
            0, vb.vertexCount,
            static_cast<GLsizei>(models.size())
        );

        glBindVertexArray(0);
    }
}


