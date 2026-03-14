#include <rendering/textures/TextureRegister.hpp>
#include <fstream>
#include <core/filesystem/image.hpp>
#include "../../../include/core/debug/gan_err.hpp"

// Created by Gordie Novak on 2/27/26.
// implementation for TextureRegister class

using namespace gan;

TextureRegister::TextureRegister()
    : textures(1) {} //< one initial texture for the 1x1 white pixel

void TextureRegister::init() {
    // "__null" represent the one white pixel.
    path_to_texture["__null"] = 0;
    alias_to_texture["__null"] = 0;

    // one white pixel
    constexpr uint8_t pixel[4] = {255, 34, 255, 90};

    // assigns our pixel color to the texture.
    textures[0] = err::unwrap(
        Texture::make(pixel, 1, 1, GAN_PIXEL, 0),
        "TextureRegister::TextureRegister()",
        "Failed to load the single magenta pixel texture into texture slot 0");
}

void TextureRegister::destroyTexture(Texture& tex) {
    if (tex.gl_id == 0 || tex.tex_id == 0)
        return;
    Texture::destroy(tex);
}

std::optional<texID> TextureRegister::createTexture(const path& path) {
    if (path_to_texture.contains(path.c_str()))
        return path_to_texture[path.c_str()];

    auto myImageOpt = image::make(path);
    if (!myImageOpt) {
        GAN_AppendLog("TextureRegister::createTexture()",
            "Failed to load image from path ", path.c_str(), ".");
        return std::nullopt;
    }

    const image myImage = std::move(myImageOpt.value());

    const auto TexOpt
        = Texture::make(
            myImage.pixels,
            myImage.w,
            myImage.h,
            default_scale_mode,
            textures.size()
        );

    if (!TexOpt) {
        GAN_AppendLog("TextureRegister::createTexture()",
            "GL failed to initialize the texture from, ", path.c_str(), ".");
        return std::nullopt;
    }

    auto tex = TexOpt.value();
    tex.tex_id = textures.size();

    textures.push_back(tex);

    path_to_texture[path.c_str()] = tex.tex_id;

    return tex.tex_id;
}

std::optional<texID> TextureRegister::createTexture(const path& path, const std::string& alias) {
    auto idOpt = createTexture(path);
    if (idOpt)
        alias_to_texture[alias] = idOpt.value();
    return idOpt;
}

std::optional<const Texture *> TextureRegister::getTexture(path file) {
    if (path_to_texture.contains(file.c_str()))
        return &textures[path_to_texture.at(file.c_str())];
    GAN_AppendLog("TextureRegister::getTexture()", "Texture \'", file.c_str(), "\' doesn't exist.");
    return std::nullopt;
}

std::optional<const Texture *> TextureRegister::getTexture(texID id) {
    if (id >= textures.size()) {
        GAN_AppendLog("TextureRegister::getTexture()",
            "Texture specified by \'", id,  "\' is out of bounds");
        return std::nullopt;
    }
    return textures.data() + id;
}

std::optional<const Texture *> TextureRegister::getTexture(std::string alias) {
    if (alias_to_texture.contains(alias))
        return &textures[path_to_texture.at(alias)];
    GAN_AppendLog("TextureRegister::getTexture()", "Texture \'", alias.c_str(), "\' doesn't exist.");
    return std::nullopt;
}

const Texture & TextureRegister::operator[](path p) {
    return textures[path_to_texture[p.c_str()]];
}


const Texture & TextureRegister::operator[](const texID id) const {
    return textures[id];
}

void TextureRegister::unloadTexture(path file) {
    if (!path_to_texture.contains(file.c_str()))
        return;
    auto& tex = textures[path_to_texture[file.c_str()]];
    destroyTexture(tex);
    for (auto& [a, t] : path_to_texture) {
        if (t == tex.gl_id) {
            path_to_texture.erase(a);
            return;
        }
    }
}

void TextureRegister::unloadTexture(std::string alias) {
    if (!alias_to_texture.contains(alias))
        return;
    auto& tex = textures[alias_to_texture[alias]];
    destroyTexture(tex);
    for (auto& [a, t] : alias_to_texture) {
        if (t == tex.gl_id) {
            alias_to_texture.erase(a);
            return;
        }
    }
}

void TextureRegister::unloadTexture(texID id) {
    if (id >= textures.size())
        return;
    destroyTexture(textures[id]);
    for (auto& [p, t] : path_to_texture) {
        if (t == id) {
            path_to_texture.erase(p);
            break;
        }
    }
    for (auto& [a, t] : alias_to_texture) {
        if (t == id) {
            alias_to_texture.erase(a);
            break;
        }
    }
}

bool TextureRegister::hasTexture(path p) const {
    return path_to_texture.contains(p.c_str());
}

bool TextureRegister::hasTexture(const std::string& alias) const {
    return (alias_to_texture.contains(alias));
}

bool TextureRegister::hasTexture(const texID id) const {
    if (id == 0) return true;
    if (id >= textures.size())
        return false;
    if (textures[id].gl_id == 0)
        return false;
    return true;
}

size_t TextureRegister::size() const {
    return path_to_texture.size();
}

void TextureRegister::setDefaultScaleMode(const ScaleMode sm) {
    default_scale_mode = sm;
}

void TextureRegister::clearAll() {
    // delete everything except the first white pixel texture.
    for (int i = 1; i < textures.size(); i++)
        glDeleteTextures(1, &textures[i].gl_id);
    textures.resize(1);
    path_to_texture.clear();
    alias_to_texture.clear();
    path_to_texture["__null"] = 0;
    alias_to_texture["__null"] = 0;
}

TextureRegister::~TextureRegister() {
    clearAll();
    // finally delete the one white pixel texture.
    glDeleteTextures(1, &textures[0].gl_id);
}


