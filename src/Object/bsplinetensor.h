#ifndef HENJIN_BSPLINETENSOR_H
#define HENJIN_BSPLINETENSOR_H

#include <glm/glm.hpp>
#include <vector>
#include "bspline.h"


class BSplineTensor {
public:
    explicit  BSplineTensor(std::vector<std::vector<glm::vec3>> &points, unsigned degreeU, unsigned degreeV, Knots knotsType=UNIFORM);

    glm::vec3 eval(float u, float v);

    float startUInterval();
    float startVInterval();

    float endUInterval();
    float endVInterval();

private:
    std::vector<std::vector<glm::vec3>> _points;

    float _startUInterval;
    float _startVInterval;

    float _endUInterval;
    float _endVInterval;

    unsigned _k;

    std::vector<unsigned> _knots;
    std::vector<Bspline> _genBspline;

};


#endif //HENJIN_BSPLINETENSOR_H

