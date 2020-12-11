#include <iostream>
#include "cylinder.h"

Cylinder::Cylinder(glm::vec3 position, float length, int subdiv1, int subdiv2) : AnimatedShape(position,glm::vec3(0,0,1)){
    glm::vec3 base = glm::vec3(0,0,0);
    glm::vec3 axis = glm::vec3(1,0,0);
    float radius = .5f;

    glm::vec3 x = glm::vec3(0,1,0);
    glm::vec3 y = cross(axis, x);

    for(int i=0; i<subdiv2; i++)
    {
        float offset = float(i)/float(subdiv2-1);
        float offset2 = (offset)*length;
        for(int j=0; j<subdiv1; j++)
        {
            glm::vec3 vert;
            glm::vec3 norm;

            float angle = 2.f*glm::pi<float>()*float(j)/float(subdiv1);

            vert = base+offset2*axis+radius*std::cos(angle)*x+radius*std::sin(angle)*y;
            norm = normalize(std::cos(angle)*x+std::sin(angle)*y);
            _mesh.addVertex(vert.x-length/2.f,vert.y,vert.z);
            _mesh.addNormal(norm);
        }
    }


    for(unsigned int i=0; i<subdiv2-1; i++)
    {
        for(unsigned int j=0; j<subdiv1; j++)
        {
            unsigned a = i*subdiv1+j;
            unsigned b = i*subdiv1+(j+1)%subdiv1;
            unsigned c = i*subdiv1+j+subdiv1;
            unsigned d = i*subdiv1+(j+1)%subdiv1+subdiv1;

            _mesh.addQuad(a,b,c,d);
        }

    }

    _mesh.load();
}

void Cylinder::draw() const {
    _mesh.draw();
}

void Cylinder::update() {
    _mesh.update(_model);
}

void Cylinder::addBone(glm::vec3 position, float lenght, glm::vec3 rotation) {

    _bones.emplace_back(std::make_shared<Bone>(position, lenght, rotation));
}

const std::vector<std::shared_ptr<Bone>> &Cylinder::bones() {
    return _bones;
}

void Cylinder::rotateBone(unsigned ind, glm::vec3 rotation) {
    _bones[ind]->rotate(rotation);
}

void Cylinder::addChildBone(unsigned ind, glm::vec3 position, float lenght, glm::vec3 rotation) {

    _bones.emplace_back(std::make_shared<Bone>(position, lenght, rotation, _bones[ind]->translation(), _bones[ind]->rotation()));
    _bones[ind]->registerChild(_bones.back());
}


