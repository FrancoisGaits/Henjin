#include "bspline.h"

Bspline::Bspline(std::vector<glm::vec3> points, unsigned int degree) : _points{points}, _k{degree+1}  {

    //vecteur uniforme
    for(unsigned i=0; i<points.size()+_k+1; ++i){
        _knots.emplace_back(i);
    }

    _startInterval = _knots[degree];
    _endInterval = _knots[points.size()];

}

glm::vec3 Bspline::eval(float u) const {
    unsigned dec = 0;
    unsigned i = _k;
    unsigned m = _k-1;

    std::vector<glm::vec3> pTemp;

    while (u > _knots[i]) {
        ++i;
        ++dec;
    }
    for(unsigned ind=dec; ind < dec + _k; ++ind) {
        pTemp.emplace_back(_points[ind]);
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

float Bspline::endInterval() const {
    return _endInterval;
}

float Bspline::startInterval() const {
    return _startInterval;
}


