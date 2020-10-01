#include "bsplinetensor.h"

BSplineTensor::BSplineTensor(std::vector<std::vector<glm::vec3>> &points, unsigned degreeU, unsigned degreeV) : _points{points} {

    for(const auto & p : points) {
        _genBspline.emplace_back(Bspline(p,degreeV));
    }

    _k = degreeU+1;

    //vecteur uniforme
    for(unsigned i=0; i<points.size()+_k+1; ++i){
        _knots.emplace_back(i);
    }

    _startUInterval = _knots[degreeU];
    _endUInterval = _knots[points.size()];

    _startVInterval = _genBspline.back().startInterval();
    _endVInterval = _genBspline.back().endInterval();



}


glm::vec3 BSplineTensor::eval(float u, float v) {
    std::vector<glm::vec3> points;
    for(const auto & bspline : _genBspline) {
        points.emplace_back(bspline.eval(v));
    }

    unsigned dec = 0;
    unsigned i = _k;
    unsigned m = _k-1;

    std::vector<glm::vec3> pTemp;

    while (u > _knots[i]) {
        ++i;
        ++dec;
    }
    for(unsigned ind=dec; ind < dec + _k; ++ind) {
        pTemp.emplace_back(points[ind]);
    }

    unsigned k = _k;

    for(unsigned l = 0; l < m; ++l) {
        for(unsigned j = 0; j < k-1; ++j) {
            pTemp[j] = (((_knots[dec+k+j] - u)/(_knots[dec+k+j] - _knots[dec+1+j])) * pTemp[j]) + (((u - _knots[dec+1+j])/(_knots[dec+k+j] - _knots[dec+1+j])) * pTemp[j+1]);
        }
        ++dec;
        --k;
    }

    return pTemp[0];
}

float BSplineTensor::startUInterval() {
    return _startUInterval;
}
float BSplineTensor::startVInterval() {
    return _startVInterval;
}

float BSplineTensor::endUInterval() {
    return _endUInterval;
}
float BSplineTensor::endVInterval() {
    return _endVInterval;
}

