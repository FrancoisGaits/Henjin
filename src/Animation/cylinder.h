#ifndef HENJIN_CYLINDER_H
#define HENJIN_CYLINDER_H


#include "src/Animation/animatedShape.h"

class Cylinder : public AnimatedShape {
public:
    explicit Cylinder(glm::vec3 base=glm::vec3(0,0,0), glm::vec3 axis = glm::vec3(1,0,0), float radius = .5f, float length = 3., int subdiv1 = 64, int subdiv2 = 512);

private:
    void draw() const override;

};


#endif //HENJIN_CYLINDER_H
