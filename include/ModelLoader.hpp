

#include "Components/Model.hpp"

class ModelLoader {
public:
    ModelLoader();
   ~ModelLoader();

    Model LoadModel(std::string path);
private:

};