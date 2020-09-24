#ifndef HENJIN_SCENE_H
#define HENJIN_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "src/Light/light.h"
#include "shader.h"
#include "camera.h"
#include "src/Object/model.h"
#include "Object/line.h"

class Scene {
public:
    explicit Scene(int width, int height);

    ~Scene() = default;

    void resize(int width, int height);
    void draw();

    void click(unsigned button, int x, int y);
    void move(int x, int y);

private:
    int _width;
    int _height;

    unsigned _button;

    glm::mat4 _view{};
    glm::mat4 _projection{};

    Camera _camera;
    Shader _shader;

    std::vector<std::unique_ptr<Model>> _objects;

    std::vector<std::unique_ptr<Line>> _lines;

    std::vector<std::unique_ptr<Light>> _lights;
};

#endif //HENJIN_SCENE_H
