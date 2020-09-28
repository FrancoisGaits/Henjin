#ifndef HENJIN_PLANE_H
#define HENJIN_PLANE_H

#include "shape.h"

class Plane : public Shape {
public:
    explicit Plane(glm::vec3 position, glm::vec3 normal, float side);

    void draw() const override;

};

#endif //HENJIN_PLANE_H
