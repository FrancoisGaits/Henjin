#ifndef HENJIN_SURFACE_H
#define HENJIN_SURFACE_H

#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"
#include "bsplinetensor.h"

struct Quad {
    unsigned a;
    unsigned b;
    unsigned c;
    unsigned d;
};

class Surface {
public:
    explicit Surface(const std::vector<std::vector<glm::vec3>>& points, glm::vec3 color = glm::vec3(1), float offset = 0.5f, float metalness=1.f, float roughness=0.5f);
    explicit Surface(const BSplineTensor& tensor, float pas, glm::vec3 color = glm::vec3(1), float offset = 0.5f, float metalness=1.f, float roughness=0.5f);

    void init(const std::vector<std::vector<glm::vec3>>& points, glm::vec3 color, float offset);

    void draw() const;

    const glm::vec3 &color() const;
    const glm::mat4 &model() const;

    inline float metalness() const {return _metalness;}
    inline float roughness() const {return _roughness;}

    void setMetalness(float metalness);
    void setRoughness(float roughness);

    void setColor(const glm::vec3& newColor);

private:

    glm::vec3 _color;
    glm::mat4 _model;
    Mesh mesh;

    float _metalness;
    float _roughness;
};

#endif //HENJIN_SURFACE_H
