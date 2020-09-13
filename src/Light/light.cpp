#include "light.h"

Light::Light(glm::vec3 position, glm::vec3 color) : _position{position}, _color{color}{

}

const glm::vec3 &Light::position() const {
    return _position;
}

const glm::vec3 &Light::color() const {
    return _color;
}

void Light::translate(const glm::vec3 &translate) {
    _position += translate;
}
