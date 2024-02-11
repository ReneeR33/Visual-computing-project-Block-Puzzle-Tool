#include "App.hpp"

//tmp for linting
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "Object.hpp"
#include "exception"
#include <iostream>

int main() {
    try
    {
        App app;
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}