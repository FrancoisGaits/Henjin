#include "animatedShape.h"

AnimatedShape::AnimatedShape(glm::vec3 position, glm::vec3 color, float scale) : _color{glm::pow(color,glm::vec3(2.2f))} {

    _model = glm::scale(glm::translate(position),glm::vec3(scale));
}

const glm::vec3 &AnimatedShape::color() const {
    return _color;
}

const glm::mat4 &AnimatedShape::model() const {
    return _model;
}

void AnimatedShape::translate(glm::vec3 vec) {
    _model = glm::translate(_model, vec);
}

void AnimatedShape::rotate(glm::vec3 rot) {
    rotateX(rot.x);
    rotateY(rot.y);
    rotateZ(rot.z);
}

void AnimatedShape::rotateX(float angle) {
    _model = glm::rotate(_model,glm::radians(angle),glm::vec3(1,0,0));
}

void AnimatedShape::rotateY(float angle) {
    _model = glm::rotate(_model,glm::radians(angle),glm::vec3(0,1,0));
}

void AnimatedShape::rotateZ(float angle) {
    _model = glm::rotate(_model,glm::radians(angle),glm::vec3(0,0,1));
}

void AnimatedShape::scale(glm::vec3 scale) {
    _model = glm::scale(_model, glm::vec3(scale));
}

void AnimatedShape::scaleX(float scale) {
    _model = glm::scale(_model, glm::vec3(scale,1,1));
}

void AnimatedShape::scaleY(float scale) {
    _model = glm::scale(_model, glm::vec3(1,scale,1));
}

void AnimatedShape::scaleZ(float scale) {
    _model = glm::scale(_model, glm::vec3(1,1,scale));
}