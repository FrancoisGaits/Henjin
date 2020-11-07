#include <iostream>
#include "metaball.h"

MetaBall::MetaBall(glm::vec3 center, float radius) : _center{center}, _radius{radius} {

}

float MetaBall::f(glm::vec3 pos) const {
    float d = glm::dot(_center - pos,_center - pos);

    return _radius/d ;
}



float MetaBalls::operator()(glm::vec3 pos) {
    float sum = 0.f;
    for(const auto & mb : _metaballs) {
        sum += mb.f(pos);
    }
    return sum;
}

void MetaBalls::addMetaBall(glm::vec3 center, float radius) {
    addMetaBall(MetaBall(center, radius));
}

void MetaBalls::addMetaBall(MetaBall mb) {
    _metaballs.emplace_back(mb);
}
