
#include "scene.h"

Scene::Scene(int width, int height) : _width(width), _height(height) {
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_MULTISAMPLE);
    glViewport(0,0,width,height);

    _shader = std::make_unique<Shader>(HDR);
    _quadShader = std::make_unique<Shader>(QUAD);
    _blurShader = std::make_unique<Shader>(BLUR);
    _camera.setviewport(glm::vec4(0.f,0.f,_width,_height));
    _view = _camera.viewmatrix();
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);

    setupSkybox();
    setupObjects();
    setupLights();
    setupQuad();
    setupShadows();

    glFinish();
}

void Scene::resize(int width, int height) {
    _width = width;
    _height = height;
    _camera.setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera.zoom(),float(_width)/float(_height),0.1f,100.f);
    setupQuad();
    setupShadows();

}

void Scene::draw(GLint qt_framebuffer, float deltaTime, float time) {
    glEnable(GL_DEPTH_TEST);
//    glClearColor(1.f,1.0f,1.0f,1.0f);

    updateScene(deltaTime, time);
    _camera.update(deltaTime);

    //shadows
    unsigned i = 0;

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    for(const auto & light : _directionalLights) {
        GLuint fbo = _depthMapFBOs[i];
        glm::mat4 lightSpaceMatrix = light->lightSpaceMatrix();

        _shadowShader.use();
        _shadowShader.setMat4fv("lightSpaceMatrix", lightSpaceMatrix);
        _shadowShader.setBool("animGPU", false);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        for(const auto &object : _objects) {
            _shadowShader.setMat4fv("model", object->model());
            object->draw();
        }

        _shadowShader.setBool("animGPU", _animGPU);
        for(const auto &object : _animatedObjects) {
            if(_animGPU) {
                for (const auto &bone : object->bones()) {
                    _shadowShader.setMat4fv("bones[" + std::to_string(bone->id()) + "]", bone->getTransform());
                }
            }
            _shadowShader.setMat4fv("model", _animGPU ? object->model() : glm::mat4{1});
            object->draw();
        }

        ++i;
    }

    //actual render
    glViewport(0,0,_width,_height);
    glBindFramebuffer(GL_FRAMEBUFFER, _quadFBO);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );




    _view = _camera.viewmatrix();

    _shader->use();

    _shader->setMat4fv("view", _view);
    _shader->setMat4fv("projection", _projection);

    _shader->setVec3("cameraPos", _camera.position());
    _shader->setInt("nbPointLight", _pointLights.size());
    _shader->setInt("nbDirectionalLight", _directionalLights.size());
    _shader->setInt("nbDirLights", _directionalLights.size());
    _shader->setFloat("exposure",_exposure);

//    _shader->setFloat("time",time);

    i = 0;
    for(const auto& light : _pointLights) {
        _shader->setVec3("pointLights[" + std::to_string(i) + "].position", light->position());
        _shader->setVec3("pointLights[" + std::to_string(i) + "].color", light->color());
        ++i;
    }

    i = 0;
    for(const auto& light : _directionalLights) {
        _shader->setVec3("directionalLights[" + std::to_string(i) + "].direction", light->direction());
        _shader->setVec3("directionalLights[" + std::to_string(i) + "].color", light->color());
        _shader->setMat4fv("lightSpaceModels[" + std::to_string(i) + "]", light->lightSpaceMatrix());
        ++i;
    }

    i = 0;
    for(const auto & depthMap : _depthMaps) {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        ++i;
    }

    for(const auto &object : _objects) {
        _shader->setMat4fv("model", object->model());
        _shader->setVec3("color", object->color());
        object->draw();
    }

    for(const auto &object : _animatedObjects) {
        for(const auto &bone : object->bones()) {
            _shader->setMat4fv("model", bone->model());
            _shader->setVec3("color", bone->color());
            if(_animGPU) {
                _shader->setMat4fv("bones[" + std::to_string(bone->id()) + "]", bone->getTransform());
            }
            bone->draw();
        }
        _shader->setBool("animGPU", _animGPU);
        _shader->setMat4fv("model", _animGPU ? object->model() : glm::mat4{1});
        _shader->setVec3("color", object->color());
        object->draw();
        _shader->setBool("animGPU", false);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



    bool horizontal = true;
    if(_bloom) {
        bool firstIteration = true;
        int amount = 10;
        _blurShader->use();
        _blurShader->setFloat("intensity",_bloomIntensity);
        for (unsigned int k = 0; k < amount; k++) {
            glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[horizontal]);
            _blurShader->setInt("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(
                    GL_TEXTURE_2D, firstIteration ? _quads[1] : _pingpong[!horizontal]
            );
            renderQuad.draw();
            horizontal = !horizontal;
            if (firstIteration)
                firstIteration = false;
        }
    }



    //skybox
    glDepthFunc(GL_LEQUAL);
    glBindFramebuffer(GL_FRAMEBUFFER, _quadFBO);
    _skyboxShader.use();
    _skyboxShader.setMat4fv("view", glm::mat4(glm::mat3(_view)));
    _skyboxShader.setMat4fv("projection", _projection);

    _skybox.bind(GL_TEXTURE0);
    _skybox.draw();
    glDepthFunc(GL_LESS);


    _quadShader->use();
    _quadShader->setBool("bloom",_bloom);
    _quadShader->setInt("toneMapping",_toneMapping);
    glBindFramebuffer(GL_FRAMEBUFFER, qt_framebuffer);
    glDisable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _quads[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _pingpong[!horizontal]);

    renderQuad.draw();

}



void Scene::click(unsigned button, int x, int y) {
    _camera.click(button,x,y);
}

void Scene::move(int x, int y, float deltaTime) {
    _camera.move(x, y, deltaTime);
}

void Scene::resetCamera() {
    _camera = Camera();
    _camera.setviewport(glm::vec4(0.f,0.f,_width,_height));
}

void Scene::setupObjects() {
    _objects.clear();
    _animatedObjects.clear();

//    ImplicitFunction funcCercle = [](glm::vec3 pos){return (pos.x * pos.x + pos.y * pos.y + pos.z * pos.z) - 0.5f;};
//    ImplicitFunction func = [](glm::vec3 pos){return (3 * pos.x * pos.x - pos.y * pos.y) + (3 * pos.z * pos.z) - 1.f;};


    switch (_sceneNumber) {
        case 0: {
            _animatedObjects.emplace_back(std::make_unique<Cylinder>(glm::vec3(1,0.5,1), 2.f));
            _animatedObjects.back()->addBone(glm::vec3{0,0.5,1});
            _animatedObjects.back()->addChildBone(0,glm::vec3{1,0,0});
            _animatedObjects.back()->rotateBone(1,glm::vec3{0,0,0});
            _animatedObjects.back()->registerBones();


//            _animatedObjects.emplace_back(std::make_unique<Cylinder>(glm::vec3(-1,0.5,1), 2.f));
//            _animatedObjects.back()->addBone(glm::vec3{0,0.5,1},1,glm::vec3{0,180,0});
//            _animatedObjects.back()->addChildBone(0,glm::vec3{1,0,0});
//            _animatedObjects.back()->registerBones();

            _objects.emplace_back(std::make_unique<Plane>(glm::vec3(0, -0.5, 0), glm::vec3(1), 10));

//            _animatedObjects.back()->rotate(glm::vec3(0,90,0));
            break;
        }

        case 1:
        {
            MetaBalls mb;
            mb.addMetaBall(glm::vec3(0), 0.9486);
            mb.addMetaBall(glm::vec3(2), 0.7746);
            mb.addMetaBall(glm::vec3(-1, 2, 1), 0.3162);
            mb.addMetaBall(glm::vec3(1, -2, -1), 0.4472);

            MetaBalls mb2;
            mb2.addMetaBall(glm::vec3(0), 0.2449);
            mb2.addMetaBall(glm::vec3(0.4), 0.1);
            mb2.addMetaBall(glm::vec3(-0.4), 0.1);
            mb2.addMetaBall(glm::vec3(-0.4, 0.4, 0.4), 0.1);
            mb2.addMetaBall(glm::vec3(0.4, -0.4, 0.4), 0.1);
            mb2.addMetaBall(glm::vec3(0.4, 0.4, -0.4), 0.1);
            mb2.addMetaBall(glm::vec3(-0.4, 0.4, -0.4), 0.1);
            mb2.addMetaBall(glm::vec3(0.4, -0.4, -0.4), 0.1);
            mb2.addMetaBall(glm::vec3(-0.4, -0.4, 0.4), 0.1);

            MetaBalls mb3;
            mb3.addMetaBall(glm::vec3(0.4), 0.08, POSITIVE, CUBE);
            mb3.addMetaBall(glm::vec3(-0.4), 0.08, POSITIVE, CUBE);
            mb3.addMetaBall(glm::vec3(-0.4, 0.4, 0.4), 0.08, POSITIVE, CUBE);
            mb3.addMetaBall(glm::vec3(0.4, -0.4, 0.4), 0.08, POSITIVE, CUBE);
            mb3.addMetaBall(glm::vec3(0.4, 0.4, -0.4), 0.08, POSITIVE, CUBE);
            mb3.addMetaBall(glm::vec3(-0.4, 0.4, -0.4), 0.08, POSITIVE, CUBE);
            mb3.addMetaBall(glm::vec3(0.4, -0.4, -0.4), 0.08, POSITIVE, CUBE);
            mb3.addMetaBall(glm::vec3(-0.4, -0.4, 0.4), 0.08, POSITIVE, CUBE);

            _objects.emplace_back(std::make_unique<IsoSurface>(mb, glm::vec3(-5, -5, -5), 0.1, glm::vec3{1, 2.2, 1},
                                                               glm::vec3{1, 0, 0}));
            _objects.back()->scale(glm::vec3(0.5));

            _objects.emplace_back(std::make_unique<Model>("aya.obj", glm::vec3(0, -0.5, 0), glm::vec3(0.5,0.5,0.5), 1, 500));

            _objects.emplace_back(std::make_unique<Plane>(glm::vec3(0, -0.5, 0), glm::vec3(0.1, 0.1, 0.1), 10));

            _objects.emplace_back(std::make_unique<IsoSurface>(mb3, glm::vec3(-1,-1,-1),0.03,glm::vec3{-4,1,1},glm::vec3(0.2,0.8,0.4)));

            _objects.emplace_back(std::make_unique<IsoSurface>(mb2, glm::vec3(-1, -1, -1), 0.03, glm::vec3{-2, 1, -1},
                                                               glm::vec3{1, 0, 0.5}));
            break;
        }

        case 2:
        {
            _objects.emplace_back(std::make_unique<Plane>(glm::vec3(0, -0.5, 0), glm::vec3(1, 1, 1), 10));
            _mb.clear();

            _mb.addMetaBall(glm::vec3(0.25), 0.1);
            _mb.addMetaBall(glm::vec3(-0.25), 0.1);
            _mb.addMetaBall(glm::vec3(-0.25, 0.25, 0.25), 0.1);
            _mb.addMetaBall(glm::vec3(-0.25, -0.25, 0.25), 0.1);
//            _mb.addMetaBall(glm::vec3(0), 0.05, NEGATIVE);

            _objects.emplace_back(std::make_unique<IsoSurface>(_mb, glm::vec3(-0.75), 0.05, glm::vec3{0, 1, 0},
                                                               glm::vec3{0.1, 0.9, 0.1}));
            break;
        }
        case 3: {
            MetaBalls mb;
            mb.addMetaBall(glm::vec3(0.4,0,0),0.25);
            mb.addMetaBall(glm::vec3(-0.4,0,0),0.25);

            _objects.emplace_back(std::make_unique<IsoSurface>(mb,glm::vec3(-1),0.05,glm::vec3{0.25,1,0},glm::vec3{1,1,1}));

            mb.moveBalls(glm::vec3{0},0.1,NEGATIVE);
            mb.addMetaBall(glm::vec3{0},0.1);
            _objects.emplace_back(std::make_unique<IsoSurface>(mb,glm::vec3(-1),0.05,glm::vec3{1,0,1},glm::vec3{1,1,1}));

            mb.clear();
            mb.addMetaBall(glm::vec3{0},0.4);
            _objects.emplace_back(std::make_unique<IsoSurface>(mb,glm::vec3(-1),0.05,glm::vec3{-2,0,-1},glm::vec3{1,1,1}));

            mb.clear();
            mb.addMetaBall(glm::vec3{0.2,0.2,0.1},0.1);
            mb.addMetaBall(glm::vec3{-0.2,0.2,0.1},0.05);
            mb.addMetaBall(glm::vec3{0.2,-0.2,-0.1},0.05);
            mb.addMetaBall(glm::vec3{-0.2,-0.2,-0.1},0.1);
            _objects.emplace_back(std::make_unique<IsoSurface>(mb,glm::vec3(-0.5),0.02,glm::vec3{-1,1,-0.5},glm::vec3{1,1,1}));

            _objects.emplace_back(std::make_unique<IsoSurface>(mb,glm::vec3(-0.5),0.02,glm::vec3{-1,-0.5,0.5},glm::vec3{1,1,1}));
            _objects.back()->rotateX(81);

            _objects.emplace_back(std::make_unique<IsoSurface>(mb,glm::vec3(-0.5),0.02,glm::vec3{0.5,-1,0.5},glm::vec3{1,1,1}));
            _objects.back()->rotateY(55);

            _objects.emplace_back(std::make_unique<IsoSurface>(mb,glm::vec3(-0.5),0.02,glm::vec3{1,0.5,-0.5},glm::vec3{1,1,1}));
            _objects.back()->rotateZ(190);


            _objects.emplace_back(std::make_unique<Plane>(glm::vec3{0,0,-30},glm::vec3{0},100000));
            _objects.back()->rotateX(90);

            _objects.emplace_back(std::make_unique<Plane>(glm::vec3{30,0,0},glm::vec3{0},100000));
            _objects.back()->rotateZ(90);

            _objects.emplace_back(std::make_unique<Plane>(glm::vec3{-30,0,0},glm::vec3{0},100000));
            _objects.back()->rotateZ(-90);

            _objects.emplace_back(std::make_unique<Plane>(glm::vec3{0,30,0},glm::vec3{0},100000));
            _objects.back()->rotateY(90);

            _objects.emplace_back(std::make_unique<Plane>(glm::vec3{0,-30,0},glm::vec3{0},100000));
            _objects.back()->rotateY(-90);

            _objects.emplace_back(std::make_unique<Plane>(glm::vec3{0,0,30},glm::vec3{0},100000));
            _objects.back()->rotateX(-90);

            break;
        }
        default:
            break;

    }
}

void Scene::setupLights() {
    _directionalLights.clear();
    _pointLights.clear();

    switch (_sceneNumber) {

        case 1 :
            _directionalLights.emplace_back(std::make_unique<DirectionalLight>(glm::vec3(1,5,1),glm::vec3(4,4,4)));
            _directionalLights.emplace_back(std::make_unique<DirectionalLight>(glm::vec3(-1,5,1),glm::vec3(1,1,1)));
            _directionalLights.emplace_back(std::make_unique<DirectionalLight>(glm::vec3(0,5,-3),glm::vec3(1,1,1)));
            break;
        case 0:
        case 2:
            _directionalLights.emplace_back(std::make_unique<DirectionalLight>(glm::vec3(1,5,1),glm::vec3(1,1,1)));
            break;

        case 3:
            _pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3{0,0,0},glm::vec3(10,10,10)));
        default:
            break;
    }
}

void Scene::setupShadows() {
    float borderColor = 1.f;

    //Shadow Mapping
    for(const auto fbo : _depthMapFBOs) {
        glDeleteFramebuffers(1,&fbo);
    }
    _depthMapFBOs.clear();

    for(const auto map : _depthMaps) {
        glDeleteTextures(1,&map);
    }
    _depthMaps.clear();


    _shader->use();
    unsigned i = 0;
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,&borderColor);

        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBOs.back());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMaps.back(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        _shader->setInt("shadowMaps["+std::to_string(i)+"]", i);

        ++i;
    }
    _shader->setInt("shadowMapSize", static_cast<int>(SHADOW_HEIGHT));
}

void Scene::reloadShader() {
    std::unique_ptr<Shader> newShader = std::make_unique<Shader>(HDR);

    if(newShader->isValid()) {
        _shader = std::move(newShader);

        _shader->use();
        _shader->setInt("shadowMapSize", static_cast<int>(SHADOW_HEIGHT));

        for(unsigned i=0; i < _directionalLights.size(); ++i) {
            _shader->setInt("shadowMaps[" + std::to_string(i) + "]", i);
        }

    }

    newShader = std::make_unique<Shader>(QUAD);
    if(newShader->isValid()) {
        _quadShader = std::move(newShader);

        _quadShader->use();
        _quadShader->setInt("hdrBuffer",0);
        _quadShader->setInt("bloomBuffer",1);

    }

    newShader = std::make_unique<Shader>(BLUR);
    if(newShader->isValid()) {
        _blurShader = std::move(newShader);

        _blurShader->use();
        _blurShader->setInt("image",0);

    }
}

void Scene::cameraKeyEvent(QKeyEvent *event) {
    _camera.keyEvent(event);
}

void Scene::cameraKeyReleaseEvent(QKeyEvent *event) {
    _camera.keyReleaseEvent(event);
}

void Scene::changeScene(unsigned sceneNumber) {
    if(sceneNumber == _sceneNumber) return;

    _sceneNumber = sceneNumber;

    setupObjects();
    setupLights();
    setupShadows();
}

void Scene::updateScene(float deltaTime, float time) {

    float sinTime = std::sin(time);
    int sinSign = sinTime>0 ? 1 : -1;

    switch (_sceneNumber) {
        case 0:
            _animatedObjects[0]->rotateBone(0,glm::vec3(0,8*deltaTime,0));
            _animatedObjects[0]->rotateBone(1,glm::vec3(0,0,35*deltaTime*sinSign));
//            _animatedObjects[1]->rotateBone(0,glm::vec3(0,8*deltaTime,0));
//            _animatedObjects[1]->rotateBone(1,glm::vec3(0,0,35*deltaTime*sinSign));
            break;
        case 1:
            _objects.back()->rotate(glm::vec3(deltaTime*90.f,deltaTime*60.f,deltaTime*45.f));
            break;
        case 2:
        {
            float sinTime = std::sin(time / 2.f);
            int sinSign = (sinTime > 0) ? 1 : ((sinTime < 0) ? -1 : 0);

            _objects.erase(_objects.end());

            _mb.moveBalls(glm::vec3(0), 0.05f * deltaTime, sinSign);
            _objects.emplace_back(std::make_unique<IsoSurface>(_mb, glm::vec3(-0.45), 0.03, glm::vec3{0, 1, 0},
                                                               glm::vec3{0.1, 0.9, 0.1}));
            _objects.back()->scale(glm::vec3(3));
            break;
        }
        default:
            break;
    }

    if(!_animGPU) {
        for (auto &object : _animatedObjects) {
            object->update();
        }
    }
}

void Scene::setupQuad() {
    //renderQuad
    glDeleteFramebuffers(1, &_quadFBO);
    glDeleteRenderbuffers(1, &_quadRBO);
    glDeleteTextures(2, _quads);

    glGenFramebuffers(1, &_quadFBO);
    glGenRenderbuffers(1, &_quadRBO);
    glGenTextures(2, _quads);

    glBindFramebuffer(GL_FRAMEBUFFER, _quadFBO);

    for(unsigned i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, _quads[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, _quads[i], 0);
    }
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    glBindRenderbuffer(GL_RENDERBUFFER, _quadRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _quadRBO);


    glGenFramebuffers(2, _pingpongFBO);
    glGenTextures(2, _pingpong);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, _pingpong[i]);
        glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, nullptr
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pingpong[i], 0
        );
    }


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    _quadShader->use();
    _quadShader->setInt("hdrBuffer",0);
    _quadShader->setInt("bloomBuffer",1);

    _blurShader->use();
    _blurShader->setInt("image",0);

}

void Scene::setBloom(bool bloom) {
    _bloom = bloom;
}

void Scene::setExposure(float exposure) {
    _exposure = exposure;
}

void Scene::setToneMapping(ToneMapping tm) {
    _toneMapping = tm;
}

bool Scene::getBloom() {
    return _bloom;
}

float Scene::getExposure() {
    return _exposure;
}

ToneMapping Scene::getToneMapping() {
    return _toneMapping;
}

void Scene::setupSkybox() {
    _skybox.load();
    _skyboxShader.use();
    _skyboxShader.setInt("gCubemapTexture",0);
}

void Scene::setBloomIntensity(float bloomIntensity) {
    _bloomIntensity = bloomIntensity;
}

float Scene::getBloomIntensity() {
    return _bloomIntensity;
}

