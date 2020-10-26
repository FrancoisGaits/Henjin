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
//    return glm::ortho<float>(-20.f, 20.f, -15.f, 15.f, -15.f, 25.f) *
//           glm::lookAt(_direction,glm::vec3(0),glm::vec3(0,1,0));

    return glm::ortho<float>(-18.f, 20.f, -12.f, 10.f, -15.f, 15.f) *
           glm::lookAt(_direction*3.f,glm::vec3(0),glm::vec3(0,1,0));
}