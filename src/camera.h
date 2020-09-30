#ifndef HENJIN_CAMERA_H
#define HENJIN_CAMERA_H

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public :
    explicit Camera(glm::vec3 position = glm::vec3(0.f,5.f,5.f), glm::vec3 up = glm::vec3(0.f,1.f,0.f), glm::vec3 look = glm::vec3(0.f), float zoom = 45.f );
    ~Camera() = default;

    glm::mat4 viewmatrix() const;
    float &zoom();
    glm::vec3 &position();

    void setviewport(glm::vec4 viewport);

    void click(unsigned button, int x, int y);
    void move(int x, int y);


private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;

    float _speed;
    float _sensitivity;
    float _radius;
    float _zoom;

    unsigned _button;

    int _startX;
    int _startY;

    glm::vec3 _rotStart;
    glm::vec3 _rotEnd;

    glm::vec2 _panStart;
    glm::vec2 _panEnd;

    glm::vec4 _viewport;

    glm::vec3 getMouseProjectionOnBall(int x, int y);
    glm::vec2 getMouseOnScreen(int x, int y);

    void rotateCamera();
    void panCamera();
};

#endif //HENJIN_CAMERA_H
