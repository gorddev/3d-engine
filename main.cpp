#define SDL_MAIN_USE_CALLBACKS 1
#define GLM_ENABLE_EXPERIMENTAL

#define OLAF_API
#include "beginner-api/camera-controllers/CamTPS.hpp"
#include "include/gan_callbacks.hpp"

#include "beginner-api/olaf-utilities/MultiObjectGeneration.hpp"

using namespace gan;

static std::vector<ShapeAPI> boxes;

int g_main(Book& book, int argc, char* argv[]) {

    boxes.push_back(book.shapes.make<Cylinder3D>(1.f, 1.f));
    boxes.push_back(book.shapes.make<Prism3D>(.5f, .5f, .5f));

    auto grid = generateGrid(book.shapes, 20);
    for (auto& g: grid) {
        boxes.push_back(g);
    }
    boxes.insert(boxes.end(), grid.begin(), grid.end());

    return 0;
}



int g_iterate(Book& book) {

    for (auto& shape : boxes) {
        book.render.draw3D(shape);
    }

    static CamTPS cam_controller(cam);

    cam_controller.useKeyboard(book.keyboard, book.clock);
    cam_controller.useMouse(book.mouse, book.clock);

    book.window.setName(std::to_string(book.clock.time).c_str());

    return 0;
}
