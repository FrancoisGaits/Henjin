#ifndef HENJIN_SHAPE_H
#define HENJIN_SHAPE_H

#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"
#include "mesh.h"

class Shape {
public:
    explicit Shape(glm::vec3 position, glm::vec3 color, float scale=1.f);

    virtual void draw() const = 0;

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

private:
    glm::vec3 _color;
    glm::mat4 _model{};

protected:
    Mesh _mesh;
};

#endif //HENJIN_SHAPE_H
