#include "plane.h"

Plane::Plane(glm::vec3 position, glm::vec3 normal, float side) : Shape(position, glm::vec3(1),side/2) {

    glm::vec3 z = glm::vec3(0,0,1);
    glm::vec3 x = glm::cross(glm::normalize(normal), z);

    glm::vec3 a = glm::normalize(z+x)*std::sqrt(2.f)/2.f;
    glm::vec3 b = glm::normalize(z-x)*std::sqrt(2.f)/2.f;
    glm::vec3 c = -b;
    glm::vec3 d = -a;

    _mesh.addVertex(a);
    _mesh.addVertex(b);
    _mesh.addVertex(c);
    _mesh.addVertex(d);

    _mesh.addNormal(normal.x, normal.y, normal.z);
    _mesh.addNormal(normal.x, normal.y, normal.z);
    _mesh.addNormal(normal.x, normal.y, normal.z);
    _mesh.addNormal(normal.x, normal.y, normal.z);

    _mesh.addQuad(0,1,2,3);

    _mesh.load();
}

void Plane::draw() const {
    _mesh.draw();
}
