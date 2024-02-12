#include "App.hpp"

#include <iostream>

int main() {
    try {
        App app;
        app.run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
}