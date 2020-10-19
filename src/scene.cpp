#include "scene.h"
#include <random>


Scene::Scene(int width, int height) : _width(width), _height(height) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glViewport(0,0,width,height);

    _camera.setviewport(glm::vec4(0.f,0.f,_width,_height));
    _view = _camera.viewmatrix();
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);

    //place_XYZ();
    create_tensor();
//    create_bspline();
    _directionallights.emplace_back(std::make_unique<DirectionalLight>(glm::vec3(1,1,-1),glm::vec3(1,1,1)));
    _directionallights.emplace_back(std::make_unique<DirectionalLight>(glm::vec3(1,-1,-1),glm::vec3(1,1,1)));

    setupShadow();
}

void Scene::resize(int width, int height) {
    _width = width;
    _height = height;
    _camera.setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);
}

void Scene::draw(GLuint qt_buffer) {
    glClearColor(1.f,1.f,1.f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned i = 0;

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    for(const auto & light : _directionallights) {
        GLuint fbo = _depthMapFBOs[i];
        glm::mat4 lightSpaceMatrix = light->lightSpaceMatrix();

        _shadowShader.use();
        _shadowShader.setMat4fv("lightSpaceMatrix", lightSpaceMatrix);


        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        for(const auto &object : _surfaces) {
            _shadowShader.setMat4fv("model", object->model());
            object->draw();
        }

        ++i;
    }


    //actual render
    glViewport(0,0,_width,_height);
    glBindFramebuffer(GL_FRAMEBUFFER, qt_buffer);

    _view = _camera.viewmatrix();

    _shader.use();

    _shader.setMat4fv("view", _view);
    _shader.setMat4fv("projection", _projection);

    _shader.setVec3("cameraPos", _camera.position());
    _shader.setInt("nbPointLight",_pointlights.size());
    _shader.setInt("nbDirectionalLight",_directionallights.size());
    _shader.setInt("nbDirLights", _directionallights.size());
    _shader.setInt("exposure",_exposure);

    i = 0;
    for(const auto& light : _pointlights) {
        _shader.setVec3("pointLights[" + std::to_string(i) + "].position", light->position());
        _shader.setVec3("pointLights[" + std::to_string(i) + "].color", light->color());
        ++i;
    }

    i = 0;
    for(const auto& light : _directionallights) {
        _shader.setVec3("directionalLights[" + std::to_string(i) + "].direction", light->direction());
        _shader.setVec3("directionalLights[" + std::to_string(i) + "].color", light->color());
        _shader.setMat4fv("lightSpaceModels[" + std::to_string(i) + "]", light->lightSpaceMatrix());
        ++i;
    }



    i = 0;
    for(const auto & depthMap : _depthMaps) {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        ++i;
    }

    for(const auto &line : _lines) {
        _shader.setMat4fv("model", line->model());
        _shader.setVec3("color", line->color());
        line->draw();
    }

    for(const auto &surface : _surfaces) {
        _shader.setFloat("metalness", surface->metalness());
        _shader.setFloat("roughness", surface->roughness());
        _shader.setMat4fv("model", surface->model());
        _shader.setVec3("color", surface->color());
        surface->draw();
    }

    for(const auto &object : _objects) {
        _shader.setMat4fv("model", object->model());
        object->draw();
    }


}



void Scene::click(unsigned button, int x, int y) {
    _button = button;
    _camera.click(button,x,y);
}

void Scene::move(int x, int y) {
    _camera.move(x,y);
}

void Scene::create_bspline() {

    std::vector<glm::vec3> points;
    points.emplace_back(glm::vec3(-5.5,0.75,-1.5));
    points.emplace_back(glm::vec3(-3.75,-1.75,2.5));
    points.emplace_back(glm::vec3(-0.25,0.75,-1.5));
    points.emplace_back(glm::vec3(3.75,-3.75,1.5));
    points.emplace_back(glm::vec3(5.75,0.75,-4.5));

    _lines.emplace_back(std::make_unique<Line>(points, true, glm::vec3(0.8,0.1,0.1)));


    std::vector<glm::vec3> points_bs;
    Bspline bs(points,3, OPEN);
    float pas = 0.05f;
    for(float u=bs.startInterval(); u<bs.endInterval(); u+=pas){
        points_bs.emplace_back(bs.eval(u));
    }

    _lines.emplace_back(std::make_unique<Line>(points_bs));
}

void Scene::create_tensor() {
    std::vector<std::vector<glm::vec3>> pointspoints;

    std::vector<glm::vec3> points;

//    points.emplace_back(glm::vec3(-10,1,-7));
//    points.emplace_back(glm::vec3(-6,-2,-7));
//    points.emplace_back(glm::vec3(-2,2,-7));
//    points.emplace_back(glm::vec3(2,-2,-7));
//    points.emplace_back(glm::vec3(6,-4,-7));
//    points.emplace_back(glm::vec3(10,1,-7));
//
//    pointspoints.emplace_back(points);
//    points.clear();
//
//    points.emplace_back(glm::vec3(-10,0,-5));
//    points.emplace_back(glm::vec3(-6,-3,-5));
//    points.emplace_back(glm::vec3(-2,3,-5));
//    points.emplace_back(glm::vec3(2,0,-5));
//    points.emplace_back(glm::vec3(6,-5,-5));
//    points.emplace_back(glm::vec3(10,0,-5));
//
//    pointspoints.emplace_back(points);
//    points.clear();
//
//    points.emplace_back(glm::vec3(-10,5,-2));
//    points.emplace_back(glm::vec3(-6,-2,-2));
//    points.emplace_back(glm::vec3(-2,1,-2));
//    points.emplace_back(glm::vec3(2,5,-2));
//    points.emplace_back(glm::vec3(6,-3,-2));
//    points.emplace_back(glm::vec3(10,0,-2));
//
//    pointspoints.emplace_back(points);
//    points.clear();
//
//    points.emplace_back(glm::vec3(-10,0,1));
//    points.emplace_back(glm::vec3(-6,-3,1));
//    points.emplace_back(glm::vec3(-2,3,1));
//    points.emplace_back(glm::vec3(2,0,1));
//    points.emplace_back(glm::vec3(6,-5,1));
//    points.emplace_back(glm::vec3(10,0,1));
//
//    pointspoints.emplace_back(points);
//    points.clear();
//
//    points.emplace_back(glm::vec3(-10,-2,4));
//    points.emplace_back(glm::vec3(-6,-5,4));
//    points.emplace_back(glm::vec3(-2,-2,4));
//    points.emplace_back(glm::vec3(2,0,4));
//    points.emplace_back(glm::vec3(6,3,4));
//    points.emplace_back(glm::vec3(10,0,4));
//
//    pointspoints.emplace_back(points);
//    points.clear();
//
//    points.emplace_back(glm::vec3(-10,4,7));
//    points.emplace_back(glm::vec3(-6,3,7));
//    points.emplace_back(glm::vec3(-2,-2,7));
//    points.emplace_back(glm::vec3(2,-5,7));
//    points.emplace_back(glm::vec3(6,0,7));
//    points.emplace_back(glm::vec3(10,-1,7));
//
//    pointspoints.emplace_back(points);
//    points.clear();


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(-25, 25);

    for(int y = -15; y < 10; ++y) {
        for(int x = -14; x < 14; ++x) {
            points.emplace_back(glm::vec3(x,(distrib(gen)/10.f),y));
        }
        pointspoints.emplace_back(points);
        points.clear();
    }


    BSplineTensor bst(pointspoints, 2, 2, OPEN);

//    for(const auto& p  : pointspoints) {
//        _lines.emplace_back(std::make_unique<Line>(p, true, glm::vec3(1,1,1)));
//    }


    _surfaces.emplace_back(std::make_unique<Surface>(bst,0.05f,glm::vec3(1, 0.4, 0.2),1.5f));
}

void Scene::place_XYZ() {
    std::vector<glm::vec3> X{glm::vec3(0,0,0),glm::vec3(1,0,0)};
    std::vector<glm::vec3> Y{glm::vec3(0,0,0),glm::vec3(0,1,0)};
    std::vector<glm::vec3> Z{glm::vec3(0,0,0),glm::vec3(0,0,1)};

    _lines.emplace_back(std::make_unique<Line>(X, true, glm::vec3(1,0,0)));
    _lines.emplace_back(std::make_unique<Line>(Y, true, glm::vec3(0,1,0)));
    _lines.emplace_back(std::make_unique<Line>(Z, true, glm::vec3(0,0,1)));


}

void Scene::setupShadow() {
    unsigned i = 0;

    _shader.use();
    for(const auto & light : _directionallights) {
        _depthMapFBOs.emplace_back(0);
        _depthMaps.emplace_back(0);
        glGenFramebuffers(1, &_depthMapFBOs.back());

        // create depth texture
        glGenTextures(1, &_depthMaps.back());
        glBindTexture(GL_TEXTURE_2D, _depthMaps.back());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBOs.back());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMaps.back(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        _shader.setInt("shadowMaps["+std::to_string(i)+"]", i);

        ++i;
    }
    _shader.setInt("shadowMapSize", static_cast<int>(SHADOW_HEIGHT));

}

void Scene::handleZoom(bool positive) {

    positive ? _camera.forward() : _camera.backward();

}

void Scene::setSurfaceColor(glm::vec3 color) {
    if(_surfaces.empty()) {
        return;
    }
    _surfaces.back()->setColor(color);
}

float Scene::changeSurfaceMetalness(bool positive) {
    if(_surfaces.empty()) {
        return 0.f;
    }

    float metalness = _surfaces.back()->metalness();
    float pas = 0.01;

    metalness += positive ? pas : -pas;
    metalness = metalness < 0.f ? 0.f : metalness;
    metalness = metalness > 1.f ? 1.f : metalness;

    _surfaces.back()->setMetalness(metalness);

    return metalness;
}

float Scene::changeSurfaceRoughness(bool positive) {
    if(_surfaces.empty()) {
        return 0.f;
    }

    float roughness = _surfaces.back()->roughness();
    float pas = 0.01;

    roughness += positive ? pas : -pas;
    roughness = roughness < 0.f ? 0.f : roughness;
    roughness = roughness > 1.f ? 1.f : roughness;

    _surfaces.back()->setRoughness(roughness);

    return roughness;
}

