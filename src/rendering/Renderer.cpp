

// created by gordie feb 16th
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "rendering/Renderer.hpp"
#include <OpenGL.h>
#include <glm/gtc/type_ptr.hpp>
#include <rendering/render-math/Model.hpp>

#include "core/EngineCore.hpp"
#include "glm/gtx/string_cast.hpp"
#include "rendering/glPrimitives/glVertex.hpp"

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

static bool needs_calc = true;

void Renderer::draw3D(Model model) {
    // add models to the model queue
    r_queue.addModel(model);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Renderer::clear(Camera& camera)  {
    // 1. Clear everything
    glClearColor(0.1f, 0.1f, 0.1f, 0.3f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& shader : shaders.shaderPrograms) {
        shader.updatePerFrameUniforms(core, camera);
    }
}

static void generateNewMatrixBuffer(const GLuint& instanceVAO, GLuint& instanceVBO) {
    glBindVertexArray(instanceVAO);
    // Generate a new matrix buffer.
    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    // Then, we make sure to set up model matrices within each shader.
    for (int i = 0; i < 4; ++i) {
        constexpr GLuint baseAttrib = 4;
        // Enable the subarray.
        glEnableVertexAttribArray(baseAttrib + i);
        // specify the size of each matrix.
        glVertexAttribPointer(
            baseAttrib + i,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::mat4),
            reinterpret_cast<void*>(sizeof(glm::vec4) * i)
        );
        glVertexAttribDivisor(baseAttrib + i, 1); //< Update on an instance-to-instance basis.
    }
}

static void generateNewColorBuffer(const GLuint& instanceVAO, GLuint& instanceVBO) {
    glBindVertexArray(instanceVAO);
    // new buffer!
    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

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

        glUseProgram(program);

        drawInstance(instances, shader);

    }
    r_queue.clearModelTree();

    SDL_GL_SwapWindow(core.window.sdl_window);
    needs_calc = true;
}

void Renderer::drawInstance(std::vector<RenderQueue::ModelInstance>& instances, const Shader& shader) {
    // go through each of our instances
    for (auto& instance : instances) {
        // first bind our vertex array

        if (!shader.requiresModelMatrix()) {
            // If our VBO isn't set up for this shape instance yet, we need to creat it.
            if (instance.instVBO == 0)
                generateNewMatrixBuffer(instance.vb.vao, instance.instVBO);

            // now we create a map to write to
            glBindBuffer(GL_ARRAY_BUFFER, instance.instVBO);

            glBufferData(
            GL_ARRAY_BUFFER,
                instance.models.size() * sizeof(glm::mat4),
                instance.models.data(),
                GL_DYNAMIC_DRAW
            );
        }

        if (!shader.requiresTexture()) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, instance.mesh.gl_tex_id);
        }

        glBindVertexArray(instance.vb.vao);
        glDrawElementsInstanced(
            GL_TRIANGLES,
            instance.vb.indexCount,
            GL_UNSIGNED_INT,
            nullptr,
            instance.models.size()
        );
        glBindVertexArray(0);
    }
}


