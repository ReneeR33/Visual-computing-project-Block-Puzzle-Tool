#ifndef TEXTURE_DATA_HPP
#define TEXTURE_DATA_HPP

#include <string>

struct TextureData {
    std::string path;

    unsigned int id;
    int width, height; 
};

#endif //TEXTURE_DATA_HPP