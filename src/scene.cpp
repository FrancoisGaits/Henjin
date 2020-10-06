#include "scene.h"

Scene::Scene(int width, int height) : _width(width), _height(height) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glViewport(0,0,width,height);

    _camera.setviewport(glm::vec4(0.f,0.f,_width,_height));
    _view = _camera.viewmatrix();
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);

    setupObjects();
    setupLights();
    setupShadows();

}

void Scene::resize(int width, int height) {
    _width = width;
    _height = height;
    _camera.setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);
}

void Scene::draw(GLint qt_framebuffer) {
    glClearColor(0.36f,0.64f,0.55f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT );

    //shadows
    unsigned i = 0;

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    for(const auto & light : _directionalLights) {
        GLuint fbo = _depthMapFBOs[i];
        glm::mat4 lightSpaceMatrix = light->lightSpaceMatrix();

        _shadowShader.use();
        _shadowShader.setMat4fv("lightSpaceMatrix", lightSpaceMatrix);


        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        for(const auto &object : _objects) {
            _shadowShader.setMat4fv("model", object->model());
            object->draw();
        }

        ++i;
    }

    //actual render
    glViewport(0,0,_width,_height);
    glBindFramebuffer(GL_FRAMEBUFFER, qt_framebuffer);

    _view = _camera.viewmatrix();

    _shader.use();

    _shader.setMat4fv("view", _view);
    _shader.setMat4fv("projection", _projection);

    _shader.setVec3("cameraPos", _camera.position());
    _shader.setInt("nbPointLight", _pointLights.size());
    _shader.setInt("nbDirectionalLight", _directionalLights.size());
    _shader.setInt("nbDirLights", _directionalLights.size());



    i = 0;
    for(const auto& light : _pointLights) {
        _shader.setVec3("pointLights[" + std::to_string(i) + "].position", light->position());
        _shader.setVec3("pointLights[" + std::to_string(i) + "].color", light->color());
        ++i;
    }

    i = 0;
    for(const auto& light : _directionalLights) {
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

    for(const auto &object : _objects) {
        _shader.setMat4fv("model", object->model());
        _shader.setVec3("color", object->color());
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

void Scene::resetCamera() {
    _camera = Camera();
    _camera.setviewport(glm::vec4(0.f,0.f,_width,_height));
}

void Scene::setupObjects() {
    _objects.emplace_back(std::make_unique<Model>("aya3.obj",glm::vec3(0,-0.5,0),glm::vec3(1),1,500));
    _objects.emplace_back(std::make_unique<Model>("aya3.obj",glm::vec3(1,-0.5,1),glm::vec3(1),1,480));
    _objects.emplace_back(std::make_unique<Model>("aya3.obj",glm::vec3(-1,-0.5,1),glm::vec3(1),1,600));
    _objects.emplace_back(std::make_unique<Plane>(glm::vec3(0,-0.5,0),glm::vec3(0,1,0),20));
}

void Scene::setupLights() {
    _directionalLights.emplace_back(std::make_unique<DirectionalLight>(glm::vec3(1,5,1),glm::vec3(1)));
//    _directionalLights.emplace_back(std::make_unique<DirectionalLight>(glm::vec3(-1,5,1),glm::vec3(1)));
//    _directionalLights.emplace_back(std::make_unique<DirectionalLight>(glm::vec3(0,5,-3),glm::vec3(1)));
//    _pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3(5), glm::vec3(0.8)));
//    _pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3(-5, 5, 5), glm::vec3(0.8, 0, 0)));
}

void Scene::setupShadows() {
    unsigned i = 0;

    _shader.use();
    for(const auto & light : _directionalLights) {
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

        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBOs.back());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMaps.back(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        _shader.setInt("shadowMaps["+std::to_string(i)+"]", i);
        ++i;
    }
}

