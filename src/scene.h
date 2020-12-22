#ifndef HENJIN_SCENE_H
#define HENJIN_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <QtGui/QKeyEvent>
#include <src/Object/renderquad.h>
#include <src/Object/skybox.h>

#include "src/Object/plane.h"
#include "src/Object/isosurface.h"
#include "src/Light/directionallight.h"
#include "src/Light/pointlight.h"
#include "shader.h"
#include "camera.h"
#include "src/Object/model.h"
#include "src/Object/metaball.h"
#include "src/Animation/cylinder.h"
#include "src/Animation/bone.h"

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
    void setBloomIntensity(float bloomIntensity);
    void setAnim(bool animOn);
    void setDisplayWeights(bool displayWeights);
    void setAnimGPU(bool animGPU);
    void setExponent(float exponent);

    bool getBloom();
    float getExposure();
    ToneMapping getToneMapping();
    float getBloomIntensity();
    float getExponent();


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
    Shader _skyboxShader = Shader(SKYBOX);
    std::unique_ptr<Shader> _shader;
    std::unique_ptr<Shader> _quadShader;
    std::unique_ptr<Shader> _blurShader;

    RenderQuad renderQuad;

    Skybox _skybox;

    std::vector<std::unique_ptr<Shape>> _objects;
    std::vector<std::unique_ptr<AnimatedShape>> _animatedObjects;
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
    float _exposure = 2;
    ToneMapping _toneMapping = FILMIC;
    float _bloomIntensity = 0.75f;

    bool _animGPU = true;

    bool _animOn = false;
    bool _displayWeights = false;

    double steps = 0;
    float _exponent = 4.f;

    void setupSkybox();
    void setupObjects();
    void setupLights();
    void setupShadows();
    void setupQuad();

    void updateScene(float deltaTime, float time);
};

#endif //HENJIN_SCENE_H
