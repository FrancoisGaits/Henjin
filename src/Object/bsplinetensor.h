#ifndef HENJIN_BSPLINETENSOR_H
#define HENJIN_BSPLINETENSOR_H

#include <glm/glm.hpp>
#include <vector>


class BSplineTensor {
public:
    explicit  BSplineTensor(std::vector<std::vector<glm::vec3>> &points, unsigned degreeU, unsigned degreeV);

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

};


#endif //HENJIN_BSPLINETENSOR_H
