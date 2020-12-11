#ifndef HENJIN_CYLINDER_H
#define HENJIN_CYLINDER_H

#include "src/Animation/animatedShape.h"
#include "bone.h"

class Cylinder : public AnimatedShape {
public:
    explicit Cylinder(glm::vec3 position, float length=1.f, int subdiv1 = 32, int subdiv2 = 48);

    void addBone(glm::vec3 position, float lenght=1.f, glm::vec3 rotation=glm::vec3{0}) override ;
    void addChildBone(unsigned ind, glm::vec3 position, float lenght=1.f, glm::vec3 rotation=glm::vec3{0}) override ;
    void update() override;
    const std::vector<std::shared_ptr<Bone>> &bones() override;

    void rotateBone(unsigned ind, glm::vec3 rotation) override;



private:
    void draw() const override;


    std::vector<std::shared_ptr<Bone>> _bones;
};


#endif //HENJIN_CYLINDER_H
