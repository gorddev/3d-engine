#pragma once

#include <beginner-api/factory/ShapeFactory.hpp>
#include <beginner-api/factory/shapes/shapedef.hpp>

/* Created by Gordie Novak on 3/1/26.
 * Purpose: 
 * Allows users to quickly create multi-object structures
 * like grids so they can get started quickly. */

namespace gan {

    inline std::vector<ShapeAPI> generateGrid(ShapeFactory& factory, int gridSize) {
        std::vector<ShapeAPI> gridBoxes;
        gridBoxes.reserve(gridSize* 3);

        for (int x = -gridSize; x <= gridSize; x++) {
            if (x == 0)
                gridBoxes.push_back(factory.make<Box3D>(0.1f, 0.1f, gridSize));
            else
                gridBoxes.push_back(factory.make<Box3D>(0.02f, 0.02f, gridSize));
            gridBoxes.back().setPos({x, 0.f, 0.f});
        }
        for (int z = -gridSize; z <= gridSize; z++) {
            if (z == 0)
                gridBoxes.push_back(factory.make<Box3D>(gridSize, 0.1f, .1f));
            else
                gridBoxes.push_back(factory.make<Box3D>(gridSize, 0.02f, 0.02f));
            gridBoxes.back().setPos({0.f, 0.f, z});
        }

        for (int y = -gridSize; y <= gridSize; y++) {
            gridBoxes.push_back(factory.make<Box3D>(0.02f, 0.02f, 1.f));
            gridBoxes.back().setPos({0.f, y, 0.f});
            gridBoxes.push_back(factory.make<Box3D>(1.f, 0.02f, 0.02f));
            gridBoxes.back().setPos({0.f, y, 0.f});
        }
        gridBoxes.push_back(factory.make<Box3D>(0.1f, gridSize, 0.1f));
        gridBoxes.back().setPos({0.f, 0.f, 0.f});

        return gridBoxes;
    }

}
