#include <iostream>
#include "metaball.h"

MetaBall::MetaBall(glm::vec3 center, float radius, BallType type) : _center{center}, _radius{radius}, _type{type} {

}

float MetaBall::f(glm::vec3 pos) const {
    float d = glm::dot(_center - pos,_center - pos);

    if(d>_threshold || d == 0.f)
        return 0;

    return (_radius/d)* static_cast<float>(_type);
}



float MetaBalls::operator()(glm::vec3 pos) {
    float sum = 0.f;
    for(const auto & mb : _metaballs) {
        sum += mb.f(pos);
    }
    return sum;
}

void MetaBalls::addMetaBall(glm::vec3 center, float radius, BallType type) {
    addMetaBall(MetaBall(center, radius, type));
}

void MetaBalls::addMetaBall(MetaBall mb) {
    _metaballs.emplace_back(mb);
}
