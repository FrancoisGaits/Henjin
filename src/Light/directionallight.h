#ifndef HENJIN_DIRECTIONALLIGHT_H
#define HENJIN_DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class DirectionalLight {
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 color);

    const glm::vec3& direction() const;
    const glm::vec3& color() const;

    glm::mat4 lightSpaceMatrix() const;

private:
    glm::vec3 _direction;
    glm::vec3 _color;
};

#endif
