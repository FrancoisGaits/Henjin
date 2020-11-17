#include "cylinder.h"

Cylinder::Cylinder(glm::vec3 base, glm::vec3 axis, float radius, float length, int subdiv1, int subdiv2) : AnimatedShape(glm::vec3(0,0.5,0),glm::vec3(0,0,1)){
    glm::vec3 x = glm::vec3(0,1,0);
    glm::vec3 y = cross(axis, x);

    for(int i=0; i<subdiv2; i++)
    {
        float offset = float(i)/float(subdiv2-1);
        float offset2 = (offset-0.5f)*length;
        for(int j=0; j<subdiv1; j++)
        {
            glm::vec3 vert;
            glm::vec3 norm;

            float angle = 2.f*glm::pi<float>()*float(j)/float(subdiv1);

            vert = base+offset2*axis+radius*std::cos(angle)*x+radius*std::sin(angle)*y;
            norm = normalize(std::cos(angle)*x+std::sin(angle)*y);
            _mesh.addVertex(vert);
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


