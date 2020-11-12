#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "metaball.h"

MetaBall::MetaBall(glm::vec3 center, float radius, Type type) : _center{center}, _orig{center}, _radius{radius}, _type{type} {

}

float MetaBall::f(glm::vec3 pos) const {
    float d = glm::dot(_center - pos,_center - pos);

    if(d>_threshold || d == 0.f)
        return 0;

    return (_radius/d)* static_cast<float>(_type);
}

void MetaBall::move(glm::vec3 pos, float intensity, int moveType) {
    glm::vec3 dir = (pos - _orig) * static_cast<float>(moveType);
    float dist = glm::length(dir);

    if(dist < 0.001f) return;
    if(glm::dot(dir, pos-_center)*moveType <= 0.f) return;

    dir /= dist;
    dir *= intensity;

    _center += dir;

}


float MetaBalls::operator()(glm::vec3 pos) {
    float sum = 0.f;
    for(const auto & mb : _metaballs) {
        sum += mb.f(pos);
    }
    return sum;
}

void MetaBalls::addMetaBall(glm::vec3 center, float radius, Type type) {
    addMetaBall(MetaBall(center, radius, type));
}

void MetaBalls::addMetaBall(MetaBall mb) {
    _metaballs.emplace_back(mb);
}

void MetaBalls::moveBalls(glm::vec3 pos, float intensity, int type) {
    for(auto & mb : _metaballs) {
        mb.move(pos,intensity,type);
    }
}

void MetaBalls::clear() {
    _metaballs.clear();
}
