#ifndef HENJIN_SCENE_H
#define HENJIN_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <QtGui/QKeyEvent>
#include <src/Object/renderquad.h>

#include "src/Object/plane.h"
#include "src/Object/isosurface.h"
#include "src/Light/directionallight.h"
#include "src/Light/pointlight.h"
#include "shader.h"
#include "camera.h"
#include "src/Object/model.h"
#include "src/Object/metaball.h"

enum ToneMapping {
    NONE = 0,
    FILMIC,
    FILMIC_LUMIN,
    REINHARD
};

class Scene {
public:
    explicit Scene(int width, int height);

    ~Scene() = default;

    void resize(int width, int height);
    void draw(GLint qt_framebuffer, float deltaTime, float time);

    void click(unsigned button, int x, int y);
    void move(int x, int y, float deltaTime);
    void resetCamera();
    void cameraKeyEvent(QKeyEvent *event);
    void cameraKeyReleaseEvent(QKeyEvent *event);
    void reloadShader();

    void changeScene(unsigned sceneNumber);
    void setBloom(bool bloom);
    void setExposure(float exposure);
    void setToneMapping(ToneMapping tm);

    bool getBloom();
    float getExposure();
    ToneMapping getToneMapping();


private:
    int _width;
    int _height;

    const unsigned SHADOW_WIDTH = 4096;
    const unsigned SHADOW_HEIGHT = 4096;

    MetaBalls _mb;

    glm::mat4 _view{};
    glm::mat4 _projection{};

    Camera _camera;
    Shader _shadowShader = Shader(SHADOW);
    std::unique_ptr<Shader> _shader;
    std::unique_ptr<Shader> _quadShader;
    std::unique_ptr<Shader> _blurShader;

    RenderQuad renderQuad;

    std::vector<std::unique_ptr<Shape>> _objects;
    std::vector<std::unique_ptr<PointLight>> _pointLights;
    std::vector<std::unique_ptr<DirectionalLight>> _directionalLights;

    std::vector<GLuint> _depthMapFBOs;
    std::vector<GLuint> _depthMaps;

    GLuint _quadFBO;
    GLuint _quadRBO;
    GLuint _quads[2];

    unsigned int _pingpongFBO[2];
    unsigned int _pingpong[2];

    unsigned _sceneNumber = 0;


    bool _bloom = false;
    float _exposure = 1;
    ToneMapping _toneMapping = FILMIC_LUMIN;

    void setupObjects();
    void setupLights();
    void setupShadows();
    void setupQuad();

    void updateScene(float deltaTime, float time);
};

#endif //HENJIN_SCENE_H
