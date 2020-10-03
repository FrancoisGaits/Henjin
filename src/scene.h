#ifndef HENJIN_SCENE_H
#define HENJIN_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "src/Object/plane.h"
#include "src/Light/directionallight.h"
#include "src/Light/pointlight.h"
#include "shader.h"
#include "camera.h"
#include "src/Object/model.h"

class Scene {
public:
    explicit Scene(int width, int height);

    ~Scene() = default;

    void resize(int width, int height);
    void draw();

    void click(unsigned button, int x, int y);
    void move(int x, int y);
    void resetCamera();

private:
    int _width;
    int _height;

    unsigned _button;

    glm::mat4 _view{};
    glm::mat4 _projection{};

    Camera _camera;
    Shader _shader;

    std::vector<std::unique_ptr<Shape>> _objects;
    std::vector<std::unique_ptr<PointLight>> _pointLights;
    std::vector<std::unique_ptr<DirectionalLight>> _directionalLights;
};

#endif //HENJIN_SCENE_H
