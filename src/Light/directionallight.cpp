#include "directionallight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color) : _direction{direction}, _color{color}{

}

const glm::vec3 &DirectionalLight::direction() const {
    return _direction;
}

const glm::vec3 &DirectionalLight::color() const {
    return _color;
}

glm::mat4 DirectionalLight::lightSpaceMatrix() const {
    return glm::ortho<float>(-15.f, 15.f, -10.f, 10.f, -10.f, 20.f) *
           glm::lookAt(_direction,glm::vec3(0),glm::vec3(0,1,0));
}