
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 look, float zoom) :  _position(position),
                                                                                _front(look - position),
                                                                                _up(up),
                                                                                _speed(5.f),
                                                                                _sensitivity(.5f),
                                                                                _zoom(zoom) {
    _worldUp = _up;
    _yaw = -90.f;
    _pitch = -40.f;

    updateVectors();
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

void Camera::click(unsigned button, int x, int y) {
    _button = button;
    _startX = x;
    _startY = y;

}

void Camera::move(int x, int y, float deltaTime) {
    float xoffset = x - _startX;
    float yoffset = _startY - y;
    _startX = x;
    _startY = y;
    xoffset *= _sensitivity;
    yoffset *= _sensitivity;
    _yaw += xoffset;
    _pitch += yoffset;

    if (_pitch > 89.0f)
        _pitch = 89.0f;
    else if (_pitch < -89.0f)
        _pitch = -89.0f;

    updateVectors();
}

void Camera::updateVectors() {

    glm::vec3 front;
    front.x = std::cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);

    _right = glm::normalize(glm::cross(_front,_worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void Camera::keyEvent(QKeyEvent *event) {
    if(_keys.find(event->key())==_keys.end()) {
        _keys.insert(event->key());
    }
}

void Camera::keyReleaseEvent(QKeyEvent *event) {
    _keys.erase(event->key());
}

void Camera::update(float deltaTime) {
    glm::vec3 dir{};
    for (const auto key : _keys) {
        switch (key) {
            case Qt::Key_Z:
                dir.x += 1;
                break;
            case Qt::Key_S:
                dir.x -= 1;
                break;
            case Qt::Key_D:
                dir.y += 1;
                break;
            case Qt::Key_Q:
                dir.y -= 1;
                break;
            case Qt::Key_Shift:
                dir.z -= 1;
                break;
            case Qt::Key_Space:
                dir.z += 1;
                break;
            default:
                break;
        }
    }
    if (glm::length(dir) > 0) {
        dir = glm::normalize(dir);
        _position += (_front * dir.x + _right * dir.y + _worldUp * dir.z) * _speed * deltaTime;
    }
}




