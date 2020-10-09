#include "pointlight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color) : _position{position}, _color{color}{

}

const glm::vec3 &PointLight::position() const {
    return _position;
}

const glm::vec3 &PointLight::color() const {
    return _color;
}

void PointLight::translate(const glm::vec3 &translate) {
    _position += translate;
}
