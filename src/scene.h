#ifndef HENJIN_SCENE_H
#define HENJIN_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include "shader.h"
#include "camera.h"
#include "src/Object/model.h"

class Scene {
public:
    explicit Scene(int width, int height);

    ~Scene() = default;

    void resize(int width, int height);
    void draw();

private:
    int _width;
    int _height;

    glm::mat4 _view;
    glm::mat4 _projection;

    Camera _camera;
    Shader _shader;
    std::unique_ptr<Model> _object;

};

#endif //HENJIN_SCENE_H
