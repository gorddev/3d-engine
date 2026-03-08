#pragma once
// Created by Gordie Novak on 3/1/26.
// holds all the includes for olaf api integration

// first we must specify that we're using the olaf api backend
#define OLAF_API //< ensures we use the "book" instead of the "engine"

// various shapes.
#include "factory/shapes/shape-apis/Box3D.hpp"
#include "factory/shapes/shape-apis/Cylinder3D.hpp"
#include "factory/shapes/shape-apis/Prism3D.hpp"

// camera controllers
#include "camera-controllers/CamFPS.hpp"
#include "camera-controllers/CamTPS.hpp"