#ifndef HENJIN_CAMERA_H
#define HENJIN_CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public :
    explicit Camera(glm::vec3 position = glm::vec3(0.f,0.f,1.f), glm::vec3 up = glm::vec3(0.f,1.f,0.f), glm::vec3 look = glm::vec3(0.f), float zoom = 45.f );
    ~Camera() = default;

    glm::mat4 viewmatrix() const;
    float &zoom();
    glm::vec3 &position();

    void setviewport(glm::vec4 viewport);


private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    float _zoom;

    glm::vec4 _viewport;
};

#endif //HENJIN_CAMERA_H
