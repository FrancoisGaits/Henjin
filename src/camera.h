#ifndef HENJIN_CAMERA_H
#define HENJIN_CAMERA_H

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QtGui/QKeyEvent>
#include <unordered_set>

class Camera {
public :
    explicit Camera(glm::vec3 position = glm::vec3(0.f,5.f,5.f), glm::vec3 up = glm::vec3(0.f,1.f,0.f), glm::vec3 look = glm::vec3(0.f), float zoom = 45.f );
    ~Camera() = default;

    glm::mat4 viewmatrix() const;
    float &zoom();
    glm::vec3 &position();

    void setviewport(glm::vec4 viewport);

    void click(unsigned button, int x, int y);
    void move(int x, int y, float deltaTime);
    void keyEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void update(float deltaTime);


private:
    std::unordered_set<int> _keys;

    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _worldUp;
    glm::vec3 _right;

    float _speed;
    float _sensitivity;
    float _zoom;
    float _yaw;
    float _pitch;

    unsigned _button;

    int _startX;
    int _startY;

    glm::vec4 _viewport;

    void updateVectors();
};

#endif //HENJIN_CAMERA_H
