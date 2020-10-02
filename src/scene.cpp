#include "scene.h"

Scene::Scene(int width, int height) : _width(width), _height(height) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glViewport(0,0,width,height);

    _camera.setviewport(glm::vec4(0.f,0.f,_width,_height));
    _view = _camera.viewmatrix();
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);

    //place_XYZ();
    create_tensor();

    _lights.emplace_back(std::make_unique<Light>(glm::vec3(5),glm::vec3(1,0,0)));
    _lights.emplace_back(std::make_unique<Light>(glm::vec3(-5,5,-5),glm::vec3(0,1,0)));
    _lights.emplace_back(std::make_unique<Light>(glm::vec3(0,5,5),glm::vec3(0,0,1)));

}

void Scene::resize(int width, int height) {
    _width = width;
    _height = height;
    _camera.setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);
}

void Scene::draw() {
    glClearColor(1.f,1.f,1.f,1.0f);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    _view = _camera.viewmatrix();

    _shader.use();

    _shader.setMat4fv("view", _view);
    _shader.setMat4fv("projection", _projection);

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
        object->draw();
    }

    for(const auto &line : _lines) {
        _shader.setMat4fv("model", line->model());
        _shader.setVec3("color", line->color());
        line->draw();
    }

    for(const auto &surface : _surfaces) {
        _shader.setMat4fv("model", surface->model());
        _shader.setVec3("color", surface->color());
        surface->draw();
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
    Bspline bs(points,2);
    float pas = 0.1f;
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

    for(int y = -10; y < 7; ++y) {
        for(int x = -10; x < 10; ++x) {
            points.emplace_back(glm::vec3(x,(rand()%20-10)/10.f,y));
        }
        pointspoints.emplace_back(points);
        points.clear();
    }


    BSplineTensor bst(pointspoints, 2, 2, REGULAR);

//    int i = 0;
//    for(const auto& p  : pointspoints) {
//        _lines.emplace_back(std::make_unique<Line>(p, true, glm::vec3(0.2*i++,0.2,0.1)));
//    }


    pointspoints.clear();

    for(float u = bst.startUInterval(); u < bst.endUInterval(); u+=0.05) {
        for(float v = bst.startVInterval(); v < bst.endVInterval(); v += 0.05) {
            glm::vec3 p = bst.eval(u,v);
            points.emplace_back(p);
        }
        pointspoints.emplace_back(points);
        points.clear();
    }

    _surfaces.emplace_back(std::make_unique<Surface>(pointspoints,glm::vec3(1)));
}

void Scene::place_XYZ() {
    std::vector<glm::vec3> X{glm::vec3(0,0,0),glm::vec3(1,0,0)};
    std::vector<glm::vec3> Y{glm::vec3(0,0,0),glm::vec3(0,1,0)};
    std::vector<glm::vec3> Z{glm::vec3(0,0,0),glm::vec3(0,0,1)};

    _lines.emplace_back(std::make_unique<Line>(X, true, glm::vec3(1,0,0)));
    _lines.emplace_back(std::make_unique<Line>(Y, true, glm::vec3(0,1,0)));
    _lines.emplace_back(std::make_unique<Line>(Z, true, glm::vec3(0,0,1)));


}
