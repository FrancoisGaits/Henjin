#include "directionallight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color) : _direction{direction}, _color{color}{

}

const glm::vec3 &DirectionalLight::direction() const {
    return _direction;
}

const glm::vec3 &DirectionalLight::color() const {
    return _color;
}