#include <src/Object/bspline.h>
#include "scene.h"

Scene::Scene(int width, int height) : _width(width), _height(height) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glViewport(0,0,width,height);

    _camera.setviewport(glm::vec4(0.f,0.f,_width,_height));
    _view = _camera.viewmatrix();
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);

    std::vector<glm::vec3> points;
    points.emplace_back(glm::vec3(-0.5,0.25,0));
    points.emplace_back(glm::vec3(-0.25,-0.25,0));
    points.emplace_back(glm::vec3(0.25,-0.25,0));
    points.emplace_back(glm::vec3(0.5,0.25,0));
    points.emplace_back(glm::vec3(-0.5,0.25,0));
    points.emplace_back(glm::vec3(-0.25,-0.25,0));
    points.emplace_back(glm::vec3(0.25,-0.25,0));
    points.emplace_back(glm::vec3(0.5,0.25,0));

    _lines.emplace_back(std::make_unique<Line>(points));


    std::vector<glm::vec3> points_bs;
    Bspline bs(points,4);
    float pas = 0.1f;
    for(float u=bs.startInterval(); u<bs.endInterval(); u+=pas){
        points_bs.emplace_back(bs.eval(u));
    }

    _lines.emplace_back(std::make_unique<Line>(points_bs));

//    _objects.emplace_back(std::make_unique<Model>("aya3.obj",glm::vec3(1),2000));
//    _objects.back()->translate(glm::vec3(-0.25,-0.25,0));

    _lights.emplace_back(std::make_unique<Light>(glm::vec3(1),glm::vec3(0.8)));
    _lights.emplace_back(std::make_unique<Light>(glm::vec3(-1,-1,1),glm::vec3(0.8,0,0)));
}

void Scene::resize(int width, int height) {
    _width = width;
    _height = height;
    _camera.setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);
}

void Scene::draw() {
    glClearColor(0.36f,0.64f,0.55f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //_view = _camera.viewmatrix();

    _shader.use();

    _shader.setVec3("cameraPos", _camera.position());
    _shader.setInt("nbLight",_lights.size());

    int i = 0;
    for(const auto& light : _lights) {
        _shader.setVec3("lights[" + std::to_string(i) + "].position", light->position());
        _shader.setVec3("lights[" + std::to_string(i) + "].color", light->color());
        ++i;
    }

    for(const auto &object : _objects) {
        _shader.setMat4fv("model", object->model());
        _shader.setMat4fv("view", _view);
        _shader.setMat4fv("projection", _projection);

        object->draw();
    }

    for(const auto &line : _lines) {
        _shader.setMat4fv("model", line->model());
        _shader.setMat4fv("view", _view);
        _shader.setMat4fv("projection", _projection);

        line->draw();
    }


}