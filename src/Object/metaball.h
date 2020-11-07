#ifndef HENJIN_METABALL_H
#define HENJIN_METABALL_H

#include <glm/glm.hpp>
#include <vector>

class MetaBall {
public:
    explicit MetaBall(glm::vec3 center, float radius);

    float f(glm::vec3 pos) const;

private:
    glm::vec3 _center;
    float _radius;


};

class MetaBalls {
public:
    explicit MetaBalls() = default;

    float operator()(glm::vec3 pos);

    void addMetaBall(MetaBall mb);
    void addMetaBall(glm::vec3 center, float radius);

private:
    std::vector<MetaBall> _metaballs;

};


#endif //HENJIN_METABALL_H
