#pragma once

#ifdef __EMSCRIPTEN__
#define GLSL_HEADER "#version 300 es\nprecision highp float;\n"
#else
#define GLSL_HEADER "#version 330 core\n"
#endif