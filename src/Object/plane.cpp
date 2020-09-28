#include "plane.h"

Plane::Plane(glm::vec3 position, glm::vec3 normal, float side) : Shape(position, glm::vec3(0), side/2) {

    glm::vec3 a = glm::vec3(0,0,1);
    glm::vec3 b = glm::cross(normal,glm::vec3(0,0,1));
    glm::vec3 c = -glm::vec3(0,0,1);
    glm::vec3 d = -glm::cross(normal,glm::vec3(0,0,1));

    _mesh.addVertex(a);
    _mesh.addVertex(b);
    _mesh.addVertex(c);
    _mesh.addVertex(d);

    _mesh.addNormal(normal.x, normal.y, normal.z);
    _mesh.addNormal(normal.x, normal.y, normal.z);
    _mesh.addNormal(normal.x, normal.y, normal.z);
    _mesh.addNormal(normal.x, normal.y, normal.z);

    _mesh.addQuad(0,1,2,3);
}

void Plane::draw() const {
    _mesh.draw();
}
