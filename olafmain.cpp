#define SDL_MAIN_USE_CALLBACKS 1
#define GLM_ENABLE_EXPERIMENTAL

#include "core/factory/shapes/ShapeAPI.hpp"
#include "core/factory/shapes/shape-apis/Box3D.hpp"
#include "core/factory/shapes/shape-apis/Cylinder3D.hpp"
#include "core/factory/shapes/shape-apis/Prism3D.hpp"

#define OLAF_API
#include "olaf-api/gan_callbacks.hpp"

using namespace gan;

static std::vector<ShapeAPI> boxes;

int g_main(Book& book, int argc, char** argv) {

    ShapeFactory shapes(book.shaders);

    boxes.push_back(shapes.make<Cylinder3D>(1.f, 1.f));
    boxes.push_back(shapes.make<Prism3D>(.5f, .5f, .5f));

    constexpr int gridsize = 20;
    for (int x = -gridsize; x <= gridsize; x++) {
        if (x == 0) {
            boxes.push_back(shapes.make<Box3D>(0.1f, 0.1f, gridsize));
        } else {
            boxes.push_back(shapes.make<Box3D>(0.02f, 0.02f, gridsize));
        }
        boxes.back().setPos({x, 0.f, 0.f});
    }
    for (int z = -gridsize; z <= gridsize; z++) {
        if (z == 0) {
            boxes.push_back(shapes.make<Box3D>(gridsize, 0.1f, .1f));
        } else {
            boxes.push_back(shapes.make<Box3D>(gridsize, 0.02f, 0.02f));
        }
        boxes.back().setPos({0.f, 0.f, z});
    }

    return 0;
}



int g_iterate(Book& book) {

    for (auto& shape : boxes) {
        book.render.draw3D(shape);
    }

    return 0;
}