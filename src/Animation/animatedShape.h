#ifndef HENJIN_ANIMATEDSHAPE_H
#define HENJIN_ANIMATEDSHAPE_H

#include <glm/glm.hpp>
#include <memory>
#include "glm/gtx/transform.hpp"
#include "animatedMesh.h"
class Bone;

class AnimatedShape {
public:
    explicit AnimatedShape(glm::vec3 position, glm::vec3 color, float scale=1.f);

    virtual void draw() const = 0;
    virtual void update() = 0;
    virtual void addBone(glm::vec3 position, float lenght=1.f, glm::vec3 rotation=glm::vec3{0}) = 0;
    virtual void addChildBone(unsigned ind, glm::vec3 position, float lenght=1.f, glm::vec3 rotation=glm::vec3{0}) = 0;
    virtual const std::vector<std::shared_ptr<Bone>> &bones() = 0;

    virtual void rotateBone(unsigned ind, glm::vec3 rotation) = 0;

    const glm::vec3 &color() const;
    const glm::mat4 &model() const;

    void translate(glm::vec3 vec);

    void rotate(glm::vec3 rot);

    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    void scale(glm::vec3 scale);

    void scaleX(float scale);
    void scaleY(float scale);
    void scaleZ(float scale);

    void registerBones(float exponent=4.f);
    void resetMesh();

private:
    glm::vec3 _color;
    glm::mat4 _id = glm::mat4{1};

protected:
    glm::mat4 _model{};
    AnimatedMesh _mesh;
};


#endif //HENJIN_ANIMATEDSHAPE_H
