#ifndef HENJIN_METABALL_H
#define HENJIN_METABALL_H

#include <glm/glm.hpp>
#include <vector>

enum BallType {
    NEGATIVE=-1,
    POSITIVE=1,
};

class MetaBall {
public:
    explicit MetaBall(glm::vec3 center, float radius, BallType type=POSITIVE);

    float f(glm::vec3 pos) const;

private:
    glm::vec3 _center;
    float _radius;
    BallType _type;

    const float _threshold = 100.f;


};

class MetaBalls {
public:
    explicit MetaBalls() = default;

    float operator()(glm::vec3 pos);

    void addMetaBall(MetaBall mb);
    void addMetaBall(glm::vec3 center, float radius, BallType type = POSITIVE);

private:
    std::vector<MetaBall> _metaballs;

};


#endif //HENJIN_METABALL_H
