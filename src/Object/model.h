#ifndef HENJIN_MODEL_H
#define HENJIN_MODEL_H

#include <string>
#include <iostream>
#include "mesh.h"
#include "shape.h"

class Model : public Shape {
public:
    explicit Model(const std::string& name, glm::vec3 position, glm::vec3 color, float scale=1.f, float div = 2000.f);

    void draw() const override;

    static Mesh loadMesh(const std::string &path, float div = 2000.f);

private:
    std::string _name;
};


#endif //HENJIN_MODEL_H
