
#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 look, float zoom) :  _position(position),
                                                                                _front(look - position),
                                                                                _up(up),
                                                                                _speed(1.f),
                                                                                _sensitivity(1.f),
                                                                                _zoom(zoom) {
    _radius = glm::length(_front);
    _front *= 1.f / _radius;

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

    switch(button) {
        case 1 :
            _rotStart = getMouseProjectionOnBall(x,y);
            _rotEnd = _rotStart;
            break;
        case 2 :
            _panStart = getMouseOnScreen(x,y);
            _panEnd = _panStart;
            break;
        default:
            break;
    }
}

void Camera::move(int x, int y) {
    switch (_button) {
        case 1 :
            _rotEnd = getMouseProjectionOnBall(x,y);
            rotateCamera();
            break;
        case 2:
            _panEnd = getMouseOnScreen(x,y);
            panCamera();
            break;
        default:
            break;
    }
}

glm::vec3 Camera::getMouseProjectionOnBall(int x, int y) {
    glm::vec3 mouseonball = glm::vec3(
            ((float)x - _viewport.z * 0.5f) / (_viewport.z * 0.5f),
            (_viewport.w * 0.5f - (float)y) / (_viewport.w * 0.5f),
            0.0f
    );
    float length = glm::length(mouseonball);

    length = (length < 1.0f) ? length : 1.0f;
    mouseonball.z = std::sqrt(1 - length * length);
    mouseonball = glm::normalize(mouseonball);
    return mouseonball;
}

glm::vec2 Camera::getMouseOnScreen(int x, int y) {
    return glm::vec2(
            ((float)x - _viewport.z * 0.5f) / (_viewport.z * 0.5f),
            ((float)y - _viewport.w * 0.5f) / (_viewport.w * 0.5f)
    );
}

void Camera::rotateCamera() {
    glm::vec3 direction = _rotEnd - _rotStart;
    float velocity = glm::length(direction);
    if (velocity > 0.0001) {
        glm::vec3 axis = glm::cross(_rotEnd, _rotStart);
        float length = glm::length(axis);
        axis = glm::normalize(axis);

        float angle = std::atan2(length, glm::dot(_rotStart, _rotEnd));

        glm::quat quaternion = glm::angleAxis(angle, axis);

        glm::vec3 center = _position + _front * _radius;
        _front = glm::normalize(glm::rotate(quaternion, _front));
        _up = glm::normalize(glm::rotate(quaternion, _up));

        _position = center - _front * _radius;
        _rotStart = _rotEnd;
    }
}

void Camera::panCamera() {
    glm::vec2 mov = _panEnd - _panStart;
    if (glm::length(mov) != 0.0f) {
        mov *= _sensitivity * _speed;
        glm::vec3 pan = glm::cross(_up, _front) * mov.x + _up * mov.y;
        _position += pan;
        _panStart = _panEnd;
    }

}




