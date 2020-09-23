#ifndef HENJIN_BSPLINE_H
#define HENJIN_BSPLINE_H

#include <glm/glm.hpp>
#include <vector>

class Bspline {

public:
    explicit Bspline(std::vector<glm::vec3> points, unsigned degree);

    glm::vec3 eval(float u);

    float startInterval();
    float endInterval();

private:
    std::vector<glm::vec3> _points;
    std::vector<float> _knots;

    float _startInterval;
    float _endInterval;
    unsigned _k;

};


#endif //HENJIN_BSPLINE_H
