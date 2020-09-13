#ifndef HENJIN_LIGHT_H
#define HENJIN_LIGHT_H

#include <glm/glm.hpp>

class Light {
public:
    Light(glm::vec3 position, glm::vec3 color);

    const glm::vec3& position() const;
    const glm::vec3& color() const;

    void translate(const glm::vec3& translate);

private:
    glm::vec3 _position;
    glm::vec3 _color;
};


#endif //HENJIN_LIGHT_H
