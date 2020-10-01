#ifndef HENJIN_SURFACE_H
#define HENJIN_SURFACE_H

#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"

class Surface {
public:
    explicit Surface(const std::vector<std::vector<glm::vec3>>& points, glm::vec3 color = glm::vec3(0));

    void draw() const;

    const glm::vec3 &color() const;
    const glm::mat4 &model() const;

private:

    glm::vec3 _color;
    glm::mat4 _model;
    Mesh mesh;
};

#endif //HENJIN_SURFACE_H
