// ReSharper disable CppNonExplicitConversionOperator
#pragma once

#include "rendering/glFunctions/glBuffers.hpp"
#include "rendering/render-math/Model.hpp"
#include "types/custom-data-structures/ref_count.hpp"

namespace gan {

    /// @brief The @code ShapeAPI@endcode class is an abstract base class that ensures that
    /// all engine shapes have a set of underlying member functions.
    /// @details Although @code ShapeAPI@endcode s are not required in order to use the renderer, they
    /// make the process significantly easier. They garbage collect themselves, manage underlying vertex
    /// buffers, and know how to render themselves.
    /// @note To render a Shape with the @code ShapeAPI@endcode, call @code render(myShapeAPI)@endcode
    /// and the shape will automatically decay to a @code Model&@endcode object for rendering.
    /// @warning You cannot create a @code ShapeAPI@endcode on the heap with the @code new@endcode keyword.
    /// This is UB behavior. Do not try to construct manually on the heap.
    struct ShapeAPI {
    protected:
        /// Underlying Vertex Buffer
        gl::VertexBuffer vb;
        /// Underlying mesh
        gl::mesh mesh;
        /// Floats represent the angle of rotation along each axis in object space
        glm::vec3 ang = glm::vec3(0.f);
    public:
        /// Position of the shape.
        glm::vec3 pos = glm::vec3(0.f);
        /// Scale in each cartesian direction (x,y,z).
        glm::vec3 scale = glm::vec3(1.f);


        /// Constructor for a ShapeAPI. Requires a model.
        explicit ShapeAPI(const Model& model)
            : vb(model.vb), mesh(model.mesh), pos(model.t.pos), scale(model.t.scale){}

        /// Destructor frees the vertex buffer allocated in the model.
        virtual ~ShapeAPI() = default;

        /// Automatic decay into a model reference for insertion into the renderer.
        [[nodiscard]] operator Model() const { // NOLINT(*-explicit-constructor)
            return Model{
                .vb = vb,
                .mesh = mesh,
                .t ={pos,{ang}, scale},
            };
        }

        // <><><> Utility functions below <><><>

        /// Rotates the model along the X-Axis by the provided angle in degrees.
        /// @param angleDeg The angle (in degrees) you want to rotate around.
        void rotateX(float angleDeg) {
            ang.x += glm::radians(angleDeg);
        }

        /// Rotates the model along the Y-Axis by the provided angle in degrees.
        /// @param angleDeg The angle (in degrees) you want to rotate around.
        void rotateY(float angleDeg) {
            ang.y += glm::radians(angleDeg);
        }

        /// Rotates the model along the X-Axis by the provided angle in degrees.
        /// @param angleDeg The angle (in degrees) you want to rotate around.
        void rotateZ(float angleDeg) {
            ang.z += glm::radians(angleDeg);
        }

        /// Rotates the model along all axes by the provided 3-vector (in degrees)
        /// @param angles The angle (in degrees) you want to rotate around.
        void rotate(const glm::vec3 &angles, const glm::vec3& axis) {
            ang += glm::radians(angles);
        }

        /// Sets the position of the model to the provided position.
        /// @param position The position you want to move the object to.
        void setPos(const glm::vec3& position)  {
            pos = position;
        }

        /// Moves the object by the provided delta. (e.g, if your position is (5, 0, 1), if you
        /// @code move(1, -1, 1)@endcode, your new position is (6, -1, 2).
        /// @param delta The amount you want to move the model by in position space.
        void move(const glm::vec3& delta)  {
            pos += delta;
        }

        /// The new operator is not valid. Pass around the ShapeAPI via reference
        /// or copies (clones).
        void* operator new(size_t i) = delete;

    };

}
