#include "plane.h"

Plane::Plane(glm::vec3 position, glm::vec3 color, float side) : Shape(position, color,side) {

    glm::vec3 normal{0,1,0};

    glm::vec3 a{-0.5,0,-0.5};
    glm::vec3 b{0.5,0,-0.5};
    glm::vec3 c{-0.5,0,0.5};
    glm::vec3 d{0.5,0,0.5};

    _mesh.addVertex(a);
    _mesh.addVertex(b);
    _mesh.addVertex(c);
    _mesh.addVertex(d);

    _mesh.addNormal(normal);
    _mesh.addNormal(normal);
    _mesh.addNormal(normal);
    _mesh.addNormal(normal);

    _mesh.addQuad(0,1,2,3);

    _mesh.load();
}

void Plane::draw() const {
    _mesh.draw();
}
