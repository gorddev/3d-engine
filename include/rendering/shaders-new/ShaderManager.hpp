#pragma once
#include "rendering/shaders/Shader.hpp"
#include "types/custom-data-structures/map_vector.hpp"

/* Created by Gordie Novak on 3/6/26.
 * Purpose: 
 * A new ShaderManager that better
 * addresses the issues that come up when creating shaders
 * allowing for easier uniform updating and management
 * of model uploading. */

namespace gan {

    class ShaderManager {

        map_vector<std::string, Shader> shaders;

    };

}
