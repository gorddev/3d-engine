#pragma once
#include <unordered_map>
#include <vector>

#include "render-primitives/Model.hpp"

// made by gordie novak on feb 24th
// sorts all the different types of objects that will Rendered so we don't
// call open GL hundreds of times.

namespace gan {
    struct RenderQueue {
        using shader_id = int;
        using texture_id = int;

        /// Vao instance contains a VertexBuffer, Texture_id, and a vector of associated models.
        struct ModelInstance {
            VertexBuffer vb;
            Mesh mesh;
            std::vector<glm::mat4> models;
            std::vector<RGBAVal> colors;
        };

        /// The tree mapping shaders to ModelInstances.
        std::unordered_map<shader_id, std::vector<ModelInstance>> modelTree;

        /// @param model The model you want to add to the model tree.
        void addModel(Model& model) {
            if (modelTree.contains(model.mesh.shader)) {
                for (auto& modelVec : modelTree[model.mesh.shader]) {
                    // If the VAO & Texture_Id match, we add them to the same ModelInstance.
                    if (model.vb.vao == modelVec.vb.vao && model.mesh.tex_id == modelVec.mesh.tex_id) {
                        // Add the transform if texture and vao match.
                        modelVec.models.push_back(model.t.compose());
                        modelVec.colors.push_back(model.t.color);
                        return; //< early return.
                    }
                }
            }
            // If there's no match, we create a new ModelInstance
            auto& vec = modelTree[model.mesh.shader];
            vec.push_back(ModelInstance{
                .vb = model.vb,
                .mesh = model.mesh,
                .models = { model.t.compose() }
            });
        }

        /// Clears all ModelInstances in the model tree.
        void clearModelTree() {
            for (auto& modelVec : modelTree) {
                for (auto& modelInst : modelVec.second) {
                    modelInst.models.clear();
                    modelInst.colors.clear();
                }
            }
        }

        /// Flush the entire model tree and clear all memory the RenderQueue uses.
        /// Not necessarily the best idea, and probably memory unsafe:
        /// @warning Matrix buffers are currently not unbound here. Oops.
        //TODO: Unbind matrix buffers after flushing the model tree.
        void flushModelTree() {
            modelTree.clear();
        }
    };
}