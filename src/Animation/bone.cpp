#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "bone.h"

Bone::Bone(unsigned Id, glm::vec3 position, float length, glm::vec3 rotation, glm::mat4 parentModel) : _id{Id}, _color{glm::vec3{0}} {

    _lenght = length;
    glm::mat4 translation = glm::translate(glm::mat4{1},position);
    _model = translation;

    _parentModel = parentModel;

    rotate(rotation);
    _inverseInitModel = glm::inverse(model());


    _origStart = _totalModel * glm::vec4{0,0,0,1};
    _origEnd = _totalModel * glm::vec4{length,0,0,1};

    _mesh.addVertex(0,0,0);
    _mesh.addNormal(0,1,0);

    _mesh.addVertex(length,0,0);
    _mesh.addNormal(0,1,0);

    _mesh.indices.push_back(0);
    _mesh.indices.push_back(1);

    _mesh.load();
}

void Bone::draw() const {
    _mesh.draw(GL_LINE_STRIP);
}

glm::mat4 Bone::getTransform() {
    return _totalModel * _inverseInitModel;
}

const glm::vec3 &Bone::origStart() {
    return _origStart;
}

const glm::vec3 &Bone::origEnd() {
    return _origEnd;
}


const glm::vec3 &Bone::color() const {
    return _color;
}

const glm::mat4 &Bone::model() const {
    return _totalModel;
}

void Bone::translate(glm::vec3 vec) {
    _model = glm::translate(_model, vec);
}

void Bone::rotate(glm::vec3 rot) {
    rotateX(rot.x);
    rotateY(rot.y);
    rotateZ(rot.z);
}

void Bone::rotateX(float angle) {
    _model = glm::rotate(_model,glm::radians(angle),glm::vec3(1,0,0));

    updateModel();
}

void Bone::rotateY(float angle) {
    _model = glm::rotate(_model,glm::radians(angle),glm::vec3(0,1,0));

    updateModel();
}

void Bone::rotateZ(float angle) {
    _model = glm::rotate(_model,glm::radians(angle),glm::vec3(0,0,1));

    updateModel();
}

void Bone::registerChild(const std::shared_ptr<Bone>& bone) {
    _children.emplace_back(bone);
    bone->setParentTransform(_totalModel);
}

void Bone::setParentTransform(glm::mat4 model) {
    _parentModel = model;

    updateModel();
}

void Bone::updateModel() {
    _totalModel = _parentModel*_model;
    for(auto & child : _children) {
        child->setParentTransform(_totalModel);
    }
}

float Bone::getDistanceFrom(glm::vec3 point) {
    glm::vec3 ab = origEnd() - origStart();
    glm::vec3 av = point - origStart();

    if(glm::dot(ab,av) <= 0) {
        return glm::length(av);
    }

    glm::vec3 bv = point - origEnd();

    if(glm::dot(ab,bv) >= 0) {
        return glm::length(bv);
    }

    return glm::length(glm::cross(ab,av))/glm::length(ab);


}

const unsigned Bone::id() const {
    return _id;
}
