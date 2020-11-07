#ifndef HENJIN_SCENE_H
#define HENJIN_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <QtGui/QKeyEvent>

#include "src/Object/plane.h"
#include "src/Object/isosurface.h"
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
    void draw(GLint qt_framebuffer);

    void click(unsigned button, int x, int y);
    void move(int x, int y);
    void resetCamera();
    void cameraKeyEvent(QKeyEvent *event, float deltaTime);
    void reloadShader();

private:
    int _width;
    int _height;

    const unsigned SHADOW_WIDTH = 2048;
    const unsigned SHADOW_HEIGHT = 2048;

    unsigned _button;


    glm::mat4 _view{};
    glm::mat4 _projection{};

    Camera _camera;
    std::unique_ptr<Shader> _shader;
    Shader _shadowShader = Shader(true);

    std::vector<std::unique_ptr<Shape>> _objects;
    std::vector<std::unique_ptr<PointLight>> _pointLights;
    std::vector<std::unique_ptr<DirectionalLight>> _directionalLights;

    std::vector<GLuint> _depthMapFBOs;
    std::vector<GLuint> _depthMaps;

    void setupObjects();
    void setupLights();
    void setupShadows();
};

#endif //HENJIN_SCENE_H
