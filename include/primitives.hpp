#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include "Model.hpp"

namespace primitives {
    static Model cube = {
        .meshes = {{
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
        }
    };
}

#endif //PRIMITIVES_HPP