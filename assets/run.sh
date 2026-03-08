cd ..
touch main.cpp
cd cmake-build-debug-emscripten
cmake --build . && emrun WebEngine.html
