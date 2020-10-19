#ifndef HENJIN_SCENE_H
#define HENJIN_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "src/Object/surface.h"
#include "src/Light/pointlight.h"
#include "src/Light/directionallight.h"
#include "src/Object/bspline.h"
#include "src/Object/bsplinetensor.h"
#include "shader.h"
#include "camera.h"
#include "src/Object/model.h"
#include "Object/line.h"

class Scene {
public:
    explicit Scene(int width, int height);

    ~Scene() = default;

    void resize(int width, int height);
    void draw(GLuint qt_buffer);

    void click(unsigned button, int x, int y);
    void move(int x, int y);

    void handleZoom(bool positive);

    void setSurfaceColor(glm::vec3 color);
    float changeSurfaceMetalness(bool positive);
    float changeSurfaceRoughness(bool positive);

    inline float getSurfaceMetalness() {return _surfaces.empty() ? 0.f : _surfaces.back()->metalness();}
    inline float getSurfaceRoughness() {return _surfaces.empty() ? 0.f : _surfaces.back()->roughness();}

private:
    void create_bspline();
    void create_tensor();
    void setupShadow();

    void defaultSurface(std::vector<std::vector<glm::vec3>>& pointspoints);
    void randomSurface(std::vector<std::vector<glm::vec3>>& pointspoints);
    void customSurface(std::vector<std::vector<glm::vec3>>& pointspoints);

    void place_XYZ();

    int _width;
    int _height;

    int _exposure = 3;

    const unsigned SHADOW_WIDTH = 2048;
    const unsigned SHADOW_HEIGHT = 2048;

    unsigned _button;

    glm::mat4 _view{};
    glm::mat4 _projection{};

    Camera _camera;
    Shader _shader;
    Shader _shadowShader = Shader(true);

    std::vector<std::unique_ptr<Model>> _objects;
    std::vector<std::unique_ptr<Line>> _lines;
    std::vector<std::unique_ptr<Surface>> _surfaces;

    std::vector<GLuint> _depthMapFBOs;
    std::vector<GLuint> _depthMaps;

    std::vector<std::unique_ptr<PointLight>> _pointlights;
    std::vector<std::unique_ptr<DirectionalLight>> _directionallights;
};

#endif //HENJIN_SCENE_H
