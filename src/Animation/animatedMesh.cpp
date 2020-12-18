#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "animatedMesh.h"

void AnimatedMesh::addQuad(unsigned a, unsigned b, unsigned c, unsigned d) {
    //triangle inferieur
    indices.emplace_back(a);
    indices.emplace_back(c);
    indices.emplace_back(d);
    //triangle superieur
    indices.emplace_back(a);
    indices.emplace_back(d);
    indices.emplace_back(b);
}

void AnimatedMesh::addTri(unsigned a, unsigned b, unsigned c) {
    indices.emplace_back(a);
    indices.emplace_back(b);
    indices.emplace_back(c);
}

void AnimatedMesh::addVertex(float x, float y, float z) {
    vertices.emplace_back(x);
    vertices.emplace_back(y);
    vertices.emplace_back(z);
}

void AnimatedMesh::addVertex(glm::vec3 &v) {
    vertices.emplace_back(v[0]);
    vertices.emplace_back(v[1]);
    vertices.emplace_back(v[2]);
}

void AnimatedMesh::addNormal(float x, float y, float z) {
    normals.emplace_back(x);
    normals.emplace_back(y);
    normals.emplace_back(z);
}

unsigned AnimatedMesh::nbTriangles() const {
    return indices.size() / 3;
}

unsigned AnimatedMesh::nbVertices() const {
    return vertices.size() / 3;
}

unsigned AnimatedMesh::nbNormals() const {
    return normals.size() / 3;
}

void AnimatedMesh::load() {
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_nbo);
    glGenBuffers(1, &_ebo);
    glGenVertexArrays(1, &_vao);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void AnimatedMesh::draw(GLenum mode) const {
    glBindVertexArray(_vao);
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void AnimatedMesh::clear() {
    vertices.clear();
    indices.clear();
    normals.clear();
}

AnimatedMesh::~AnimatedMesh() {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_nbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
}

void AnimatedMesh::addNormal(glm::vec3 &n) {
    addNormal(n.x,n.y,n.z);
}

void AnimatedMesh::update(glm::mat4 meshModel) {

    std::vector<GLfloat> newVertices;
    std::vector<GLfloat> newNormals;

    for(unsigned i=0; i < vertices.size(); i+=3) {
        glm::vec4 vertex{vertices[i],vertices[i+1],vertices[i+2],1};
        glm::vec4 normal{normals[i],normals[i+1],normals[i+2],1};
        glm::mat4 accV{0};
        glm::mat4 accN{0};

        for (const auto &p : _bonesNweights) {
            accV += p.second[i/3] * p.first->getTransform();
            accN += p.second[i/3] * glm::inverse(glm::transpose(p.first->getTransform()));
        }
        glm::vec4 newVertex = accV*meshModel*vertex;
        glm::vec4 newNormal = accN*glm::inverse(glm::transpose(meshModel))*normal;

        newVertices.emplace_back(newVertex.x);
        newVertices.emplace_back(newVertex.y);
        newVertices.emplace_back(newVertex.z);

        newNormals.emplace_back(newNormal.x);
        newNormals.emplace_back(newNormal.y);
        newNormals.emplace_back(newNormal.z);


//        newVertices.emplace_back(newVertex.x/newVertex.w);
//        newVertices.emplace_back(newVertex.y/newVertex.w);
//        newVertices.emplace_back(newVertex.z/newVertex.w);
//

    }

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, newVertices.size() * sizeof(GLfloat), newVertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, newNormals.size() * sizeof(GLfloat), newNormals.data());

    glBindVertexArray(0);
}

void AnimatedMesh::submitBones(const std::vector<std::shared_ptr<Bone>>& bones, glm::mat4 objectModel) {
    std::cout << glm::to_string(objectModel) << std::endl;

    for (unsigned i = 0; i < vertices.size(); i += 3) {
        glm::vec4 v{vertices[i],vertices[i+1],vertices[i+2],1};
        glm::vec3 vert = objectModel*v;
        float sum = 0.f;
        std::vector<float> w;

        for(const auto & bone : bones) {
            float dist = 1.f/std::pow(bone->getDistanceFrom(vert),4.f);

            w.emplace_back(dist);
            sum += dist;
        }

        unsigned j = 0;
        for(const auto & bone : bones) {
            _bonesNweights[bone].emplace_back(w[j++]/sum);
        }
    }














//    for(const auto & bone : bones) {
//        for(const auto & w : _bonesNweights[bone]) {
//            std::cout << w << "  |  ";
//        }
//        std::cout << std::endl;
//    }


}

