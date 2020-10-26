#include <iostream>
#include "bsplinetensor.h"

BSplineTensor::BSplineTensor(std::vector<std::vector<glm::vec3>> &points, unsigned degreeU, unsigned degreeV, Knots knotType) : _points{points} {

    for(const auto & p : points) {
        _dirBsplines.emplace_back(Bspline(p, degreeV, knotType));
    }

    _k = degreeU+1;

    switch(knotType) {
        case OPEN : {
            unsigned j = 0;
            for (unsigned i = 0; i < points.size() + _k + 1; ++i) {
                _knots.emplace_back(j);
                if(i >= degreeU and i < points.size()) {
                    ++j;
                }
            }

            break;
        }
        default :
            for (unsigned i = 0; i < points.size() + _k + 1; ++i) {
                _knots.emplace_back(i);
            }
    }

    _startUInterval = _knots[degreeU];
    _endUInterval = _knots[points.size()];

    _startVInterval = _dirBsplines.back().startInterval();
    _endVInterval = _dirBsplines.back().endInterval();



}


glm::vec3 BSplineTensor::eval(float u, float v) const {
    std::vector<glm::vec3> points;
    for(const auto & bspline : _dirBsplines) {
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

float BSplineTensor::startUInterval() const {
    return _startUInterval;
}
float BSplineTensor::startVInterval() const {
    return _startVInterval;
}

float BSplineTensor::endUInterval() const {
    return _endUInterval;
}
float BSplineTensor::endVInterval() const {
    return _endVInterval;
}

