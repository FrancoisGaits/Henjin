#ifndef HENJIN_METABALL_H
#define HENJIN_METABALL_H

#include <glm/glm.hpp>
#include <vector>

enum ShapeType {
    SPHERE = 1,
    CUBE
};

enum ContribType {
    NEGATIVE=-1,
    POSITIVE=1,
};

class MetaBall {
public:
    explicit MetaBall(glm::vec3 center, float radius, ContribType Ctype=POSITIVE, ShapeType Stype=SPHERE);

    float f(glm::vec3 pos) const;
    void move(glm::vec3 pos, float intensity, int moveType);

private:
    glm::vec3 _center;
    glm::vec3 _orig;
    float _radius;
    ContribType _Ctype;
    ShapeType _Stype;

    const float _threshold = 100.f;



};

class MetaBalls {
public:
    explicit MetaBalls() = default;

    float operator()(glm::vec3 pos);

    void addMetaBall(MetaBall mb);
    void addMetaBall(glm::vec3 center, float radius, ContribType Ctype = POSITIVE, ShapeType Stype=SPHERE);

    void moveBalls(glm::vec3 pos, float intensity, int type);

    void clear();

private:
    std::vector<MetaBall> _metaballs;



};


#endif //HENJIN_METABALL_H
