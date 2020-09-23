#ifndef HENJIN_LINE_H
#define HENJIN_LINE_H

#include <glm/glm.hpp>
#include <string>
#include "mesh.h"
#include "glm/gtc/matrix_transform.hpp"

class Line {

public:
    explicit Line(const std::vector<glm::vec3>& points, glm::vec3 color = glm::vec3(1));

    void draw() const;

    const glm::vec3 &color() const;
    const glm::mat4 &model() const;

    void translate(glm::vec3 vec);

private:
    std::string _name;

    glm::vec3 _color;
    glm::mat4 _model;
    Mesh mesh;
};


#endif //HENJIN_LINE_H
