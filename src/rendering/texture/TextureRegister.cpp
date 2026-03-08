#include <rendering/textures/TextureRegister.hpp>
#include <rendering/glFunctions/glTextures.hpp>
#include <fstream>
#include <core/filesystem/image.hpp>
#include "../../../include/core/debug/gan_io.hpp"

// Created by Gordie Novak on 2/27/26.
// implementation for TextureRegister class

using namespace gan;

TextureRegister::TextureRegister()
    : textures(1) {}

void TextureRegister::init() {
    // "__null" represent the one magenta pixel.
    path_to_texture["__null"] = 0;
    alias_to_texture["__null"] = 0;

    // set up the very first texture.
    textures[0].id = 0;
    textures[0].w = 1;
    textures[0].h = 1;

    // one magenta pixel
    const uint8_t pixel[3] = {255, 0, 255};

    // assigns our pixel color to the texture.
    textures[0].gl_id = err::unwrap(
        gl::generate2DTexture(&pixel, 1, 1, PIXEL),
        "TextureRegister::TextureRegister()",
        "Failed to load the single magenta pixel texture into texture slot 0");
}

void TextureRegister::destroyTexture(Texture& tex) {
    if (tex.gl_id == 0)
        return;
    glDeleteTextures(1, &tex.gl_id);
    tex.w = 0;
    tex.h = 0;
    tex.gl_id = 0;
}

std::optional<texID> TextureRegister::createTexture(const path& path) {
    if (path_to_texture.contains(path.c_str()))
        return path_to_texture[path.c_str()];

    auto myImageOpt = image::make(path);
    if (!myImageOpt) {
        GAN_AppendError("TextureRegister::createTexture()",
            "Failed to load image from path ", path.c_str(), ".");
        return std::nullopt;
    }

    const image myImage = std::move(myImageOpt.value());

    const auto GLidOpt
        = gl::generate2DTexture(
            myImage.pixels,
            myImage.w,
            myImage.h,
            default_scale_mode
        );

    if (!GLidOpt) {
        GAN_AppendError("TextureRegister::createTexture()",
            "GL failed to initialize the texture from, ", path.c_str(), ".");
        return std::nullopt;
    }

    auto GLid = GLidOpt.value();

    texID index = textures.size();

    textures.push_back({
        index, GLid, myImage.w, myImage.h
    });

    path_to_texture[path.c_str()] = index;

    return index;
}

std::optional<texID> TextureRegister::createTexture(path path, std::string alias) {
    auto idOpt = createTexture(path);
    if (idOpt)
        alias_to_texture[alias] = idOpt.value();
    return idOpt;
}

std::optional<const Texture *> TextureRegister::getTexture(path file) {
    if (path_to_texture.contains(file.c_str()))
        return &textures[path_to_texture.at(file.c_str())];
    GAN_AppendError("TextureRegister::getTexture()", "Texture \'", file.c_str(), "\' doesn't exist.");
    return std::nullopt;
}

std::optional<const Texture *> TextureRegister::getTexture(texID id) {
    if (id >= textures.size()) {
        GAN_AppendError("TextureRegister::getTexture()",
            "Texture specified by \'", id,  "\' is out of bounds");
        return std::nullopt;
    }
    return textures.data() + id;
}

std::optional<const Texture *> TextureRegister::getTexture(std::string alias) {
    if (alias_to_texture.contains(alias))
        return &textures[path_to_texture.at(alias)];
    GAN_AppendError("TextureRegister::getTexture()", "Texture \'", alias.c_str(), "\' doesn't exist.");
    return std::nullopt;
}

const Texture & TextureRegister::operator[](path p) {
    return textures[path_to_texture[p.c_str()]];
}

const Texture & TextureRegister::operator[](const std::string& alias) {
    return textures[alias_to_texture[alias.c_str()]];
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
        if (t == tex.id) {
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
        if (t == tex.id) {
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

bool TextureRegister::hasTexture(path p) {
    return path_to_texture.contains(p.c_str());
}

bool TextureRegister::hasTexture(std::string alias) {
    return (alias_to_texture.contains(alias));
}

bool TextureRegister::hasTexture(texID id) {
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
    for (int i = 0; i < textures.size(); i++)
        glDeleteTextures(1, &textures[i].gl_id);
    textures.resize(1);
    path_to_texture.clear();
    alias_to_texture.clear();
    path_to_texture["__null"] = 0;
    alias_to_texture["__null"] = 0;
}

TextureRegister::~TextureRegister() {
    clearAll();
}


