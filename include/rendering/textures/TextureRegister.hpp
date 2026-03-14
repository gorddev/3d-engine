#pragma once
#include <unordered_map>
#include <vector>

#include "core/filesystem/path.hpp"
#include "Texture.hpp"
#include "types/stack.hpp"
#include "types/enums/RenderingSettings.hpp"
// Created by Gordie Novak on 2/27/26.
// used to keep track of textures and assign them unique ids.

namespace gan {

    /// The zero-ith texture is always just one magenta pixel.
    class TextureRegister {
        /// Storage containers for all textures.
        std::vector<Texture> textures;
        /// Maps the actual path (like "assets/test.png") to the index in the vector.
        std::unordered_map<std::string, texID> path_to_texture;
        /// Maps a given alias (like "player texture") to the index in the vector.
        std::unordered_map<std::string, texID> alias_to_texture;
        /// Default sampling mode
        ScaleMode default_scale_mode = GAN_LINEAR;

        TextureRegister();                          ///< Private constructor.

        static void destroyTexture(Texture& tex);   ///< Internal private destroy texture function.

        void init();                                ///< Creates a white 1x1 pixel texture.

        friend class EngineCore;
        friend class Engine;
    public:
        std::optional<texID> createTexture(const path&);                ///< Creates tex from given path. Returns ID
        std::optional<texID> createTexture(const path&, const std::string& alias);    ///< Creates tex & assigns alias. Returns ID

        std::optional<const Texture*> getTexture(path);         ///< Safe
        std::optional<const Texture*> getTexture(texID);        ///< Safe
        std::optional<const Texture*> getTexture(std::string);  ///< Safe

        [[nodiscard]] const Texture& operator[](path);          ///< @warning No bounds check
        [[nodiscard]] const Texture& operator[](texID) const;   ///< @warning No bounds check

        void unloadTexture(path);                               ///< @note Does nothing if tex doesn't exist.
        void unloadTexture(std::string alias);                  ///< @note Does nothing if tex doesn't exist.
        void unloadTexture(texID);                              ///< @note Does nothing if tex doesn't exist.

        [[nodiscard]] bool hasTexture(path) const;               ///< Returns true if tex-reg has texture from the path
        [[nodiscard]] bool hasTexture(const std::string&) const; ///< Returns true if alias has texture in the tex-reg
        [[nodiscard]] bool hasTexture(texID) const;              ///< Returns true if the texture with the given ID exists

        [[nodiscard]] size_t size() const;          ///< Returns the number of textures currently allocated.

        void setDefaultScaleMode(ScaleMode);        ///< Sets the scale mode new textures will be created with.

        void clearAll();                            ///< Deallocates all textures in the textureRegister

        ~TextureRegister();

        // Ensures no moving/copying
        TextureRegister(const TextureRegister&) = delete;   /// Nope. You can't do that.
        auto operator=(const TextureRegister&)  = delete;   /// Nope. You can't do that.
        TextureRegister(TextureRegister&&)      = delete;   /// Nope. You can't do that.
        auto operator=(TextureRegister&&)       = delete;   /// Nope. You can't do that.
    };

}