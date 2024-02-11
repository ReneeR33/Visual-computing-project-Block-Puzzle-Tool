#ifndef CUBE_HPP
#define CUBE_HPP

#include "Model.hpp"

struct Cube : public Model {
    Cube() {
        this->name = "cube";
        this->meshes = {{
                .vertices = {
                    { .position = {-0.5f, -0.5f, -0.5f}, .normal = {0.0f, 0.0f, -1.0f}},
                    { .position = {0.5f, -0.5f, -0.5f},  .normal = {0.0f, 0.0f, -1.0f}},
                    { .position = {0.5f,  0.5f, -0.5f},  .normal = {0.0f, 0.0f, -1.0f}},
                    { .position = {0.5f,  0.5f, -0.5f},  .normal = {0.0f, 0.0f, -1.0f}},
                    { .position = {-0.5f,  0.5f, -0.5f}, .normal = {0.0f, 0.0f, -1.0f}},
                    { .position = {-0.5f, -0.5f, -0.5f}, .normal = {0.0f, 0.0f, -1.0f}},

                    { .position = {-0.5f, -0.5f,  0.5f}, .normal = {0.0f, 0.0f, 1.0f}},
                    { .position = {0.5f, -0.5f,  0.5f},  .normal = {0.0f, 0.0f, 1.0f}},
                    { .position = {0.5f,  0.5f,  0.5f},  .normal = {0.0f, 0.0f, 1.0f}},
                    { .position = {0.5f,  0.5f,  0.5f},  .normal = {0.0f, 0.0f, 1.0f}},
                    { .position = {-0.5f,  0.5f,  0.5f}, .normal = {0.0f, 0.0f, 1.0f}},
                    { .position = {-0.5f, -0.5f,  0.5f}, .normal = {0.0f, 0.0f, 1.0f}},

                    { .position = {-0.5f,  0.5f,  0.5f}, .normal = {-1.0f, 0.0f,  0.0f}},
                    { .position = {-0.5f,  0.5f, -0.5f}, .normal = {-1.0f, 0.0f,  0.0f}},
                    { .position = {-0.5f, -0.5f, -0.5f}, .normal = {-1.0f, 0.0f,  0.0f}},
                    { .position = {-0.5f, -0.5f, -0.5f}, .normal = {-1.0f, 0.0f,  0.0f}},
                    { .position = {-0.5f, -0.5f,  0.5f}, .normal = {-1.0f, 0.0f,  0.0f}},
                    { .position = {-0.5f,  0.5f,  0.5f}, .normal = {-1.0f, 0.0f,  0.0f}},

                    { .position = {0.5f,  0.5f,  0.5f}, .normal = {1.0f,  0.0f,  0.0f}},
                    { .position = {0.5f,  0.5f, -0.5f}, .normal = {1.0f,  0.0f,  0.0f}},
                    { .position = {0.5f, -0.5f, -0.5f}, .normal = {1.0f,  0.0f,  0.0f}},
                    { .position = {0.5f, -0.5f, -0.5f}, .normal = {1.0f,  0.0f,  0.0f}},
                    { .position = {0.5f, -0.5f,  0.5f}, .normal = {1.0f,  0.0f,  0.0f}},
                    { .position = {0.5f,  0.5f,  0.5f}, .normal = {1.0f,  0.0f,  0.0f}},

                    { .position = {-0.5f, -0.5f, -0.5f}, .normal = {0.0f, -1.0f,  0.0f}},
                    { .position = {0.5f, -0.5f, -0.5f},  .normal = {0.0f, -1.0f,  0.0f}},
                    { .position = {0.5f, -0.5f,  0.5f},  .normal = {0.0f, -1.0f,  0.0f}},
                    { .position = {0.5f, -0.5f,  0.5f},  .normal = {0.0f, -1.0f,  0.0f}},
                    { .position = {-0.5f, -0.5f,  0.5f}, .normal = {0.0f, -1.0f,  0.0f}},
                    { .position = {-0.5f, -0.5f, -0.5f}, .normal = {0.0f, -1.0f,  0.0f}},

                    { .position = {-0.5f,  0.5f, -0.5f}, .normal = {0.0f,  1.0f,  0.0f}},
                    { .position = {0.5f,  0.5f, -0.5f},  .normal = {0.0f,  1.0f,  0.0f}},
                    { .position = {0.5f,  0.5f,  0.5f},  .normal = {0.0f,  1.0f,  0.0f}},
                    { .position = {0.5f,  0.5f,  0.5f},  .normal = {0.0f,  1.0f,  0.0f}},
                    { .position = {-0.5f,  0.5f,  0.5f}, .normal = {0.0f,  1.0f,  0.0f}},
                    { .position = {-0.5f,  0.5f, -0.5f}, .normal = {0.0f,  1.0f,  0.0f}}
                }
            }
        };
    }
};


#endif //CUBE_HPP