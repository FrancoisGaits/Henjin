#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 look, float zoom) :  _position(position),
                                                                                _front(look - position),
                                                                                _up(up),
                                                                                _speed(0.1),
                                                                                _zoom(zoom) {

}

glm::mat4 Camera::viewmatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

float &Camera::zoom() {
    return _zoom;
}

glm::vec3 &Camera::position() {
    return _position;
}

void Camera::setviewport(glm::vec4 viewport) {
    _viewport = viewport;
}




