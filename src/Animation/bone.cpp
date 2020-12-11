#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "bone.h"

Bone::Bone(glm::vec3 position, float length, glm::vec3 rotation, glm::mat4 parentTranslation,
           glm::mat4 parentRotation) : _color{glm::vec3{0}} {

    _lenght = length;
    _translation = glm::translate(glm::mat4{1},position);
    _rotation = glm::mat4{1};
    _parentRotation = parentRotation;
    _parentTranslation = parentTranslation;

    rotate(rotation);
    _inverseInitModel = glm::inverse(model());


    _origStart = _totalModel * glm::vec4{0,0,0,1};
    _origEnd = _totalModel * glm::vec4{length,0,0,1};

//    std::cout << "ORIG POS : " << glm::to_string(_origStart) << glm::to_string(origEnd()) << std::endl;

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
    _translation = glm::translate(_translation, vec);
}

void Bone::rotate(glm::vec3 rot) {
    rotateX(rot.x);
    rotateY(rot.y);
    rotateZ(rot.z);
}

void Bone::rotateX(float angle) {
    _rotation = glm::rotate(_rotation,glm::radians(angle),glm::vec3(1,0,0));

    updateModel();
}

void Bone::rotateY(float angle) {
    _rotation = glm::rotate(_rotation,glm::radians(angle),glm::vec3(0,1,0));

    updateModel();
}

void Bone::rotateZ(float angle) {
    _rotation = glm::rotate(_rotation,glm::radians(angle),glm::vec3(0,0,1));

    updateModel();
}

void Bone::registerChild(const std::shared_ptr<Bone>& bone) {
    _children.emplace_back(bone);
    bone->setParentTransform(_translation*_parentTranslation,_rotation*_parentTranslation);
}

void Bone::setParentTransform(glm::mat4 translation, glm::mat4 rotation) {
    _parentTranslation = translation;
    _parentRotation = rotation;

    updateModel();
}

void Bone::updateModel() {
    _totalModel = _parentTranslation*_parentRotation*_translation*_rotation;
    for(auto & child : _children) {
        child->setParentTransform(_translation*_parentTranslation,_rotation*_parentTranslation);
    }
}

const glm::mat4 &Bone::translation() const{
    return _translation;
}

const glm::mat4 &Bone::rotation() const{
    return _rotation;
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

//float Bone::getDistanceFrom(glm::vec3 point) {
//    float lengthSq = std::pow(_lenght,2.f);
//    float t = ((point.x - origStart().x) * (origEnd().x - origStart().x) + (point.y - origStart().y) * (origEnd().y - origStart().y) + (point.z - origStart().z) * (origEnd().z - origStart().z)) / lengthSq;
//
//    t = std::max(1.f,std::min(0.f,t));
//
//    glm::vec3 proj{origStart() + t*(origEnd()-origStart())};
//
////    if(point.z==0) {
////        std::cout << "DIST : " << glm::to_string(point) << " and " << glm::to_string(origStart()) << " | "
////                  << glm::to_string(origEnd()) << " = " << glm::distance(point, proj) << std::endl;
////    }
//    return glm::distance(point,proj);
//}
////
////float dist_to_segment_squared(float px, float py, float pz, float lx1, float ly1, float lz1, float lx2, float ly2, float lz2) {
////    float line_dist = dist_sq(lx1, ly1, lz1, lx2, ly2, lz2);
////    if (line_dist == 0) return dist_sq(px, py, pz, lx1, ly1, lz1);
////    float t = ((px - lx1) * (lx2 - lx1) + (py - ly1) * (ly2 - ly1) + (pz - lz1) * (lz2 - lz1)) / line_dist;
////    t = constrain(t, 0, 1);
////    return dist_sq(px, py, pz, lx1 + t * (lx2 - lx1), ly1 + t * (ly2 - ly1), lz1 + t * (lz2 - lz1));
////}

