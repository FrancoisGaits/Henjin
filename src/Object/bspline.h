#ifndef HENJIN_BSPLINE_H
#define HENJIN_BSPLINE_H

#include <glm/glm.hpp>
#include <vector>

enum Knots {
    UNIFORM,
    OPEN
};

class Bspline {

public:
    explicit Bspline(std::vector<glm::vec3> points, unsigned degree, Knots knotType = UNIFORM);

    glm::vec3 eval(float u) const;

    float startInterval() const;
    float endInterval() const;

private:
    std::vector<glm::vec3> _points;
    std::vector<float> _knots;

    float _startInterval;
    float _endInterval;
    unsigned _k;

};


#endif //HENJIN_BSPLINE_H