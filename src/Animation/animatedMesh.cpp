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
    glGenBuffers(1, &_bbo);
    glGenBuffers(1, &_wbo);
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
    glDeleteBuffers(1, &_bbo);
    glDeleteBuffers(1, &_wbo);
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
        glm::vec4 normal{normals[i],normals[i+1],normals[i+2],0};
        unsigned vertId = i/3;
        glm::mat4 accV{0};

        glm::ivec4 bonesId = glm::ivec4(_bonesIDs[vertId*4],_bonesIDs[vertId*4+1],_bonesIDs[vertId*4+2],_bonesIDs[vertId*4+3]);
        glm::vec4 weights = glm::vec4(_bonesWeights[vertId*4],_bonesWeights[vertId*4+1],_bonesWeights[vertId*4+2],_bonesWeights[vertId*4+3]);

        for(unsigned j=0; j<4 && bonesId[j] != -1; ++j) {
            accV += weights[j] * _bones[bonesId[j]]->getTransform();
        }

        glm::vec4 newVertex = accV*meshModel*vertex;
        glm::vec4 newNormal = glm::transpose(glm::inverse(accV*meshModel))*normal;

        newVertices.emplace_back(newVertex.x);
        newVertices.emplace_back(newVertex.y);
        newVertices.emplace_back(newVertex.z);

        newNormals.emplace_back(newNormal.x);
        newNormals.emplace_back(newNormal.y);
        newNormals.emplace_back(newNormal.z);


    }

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, newVertices.size() * sizeof(GLfloat), newVertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, newNormals.size() * sizeof(GLfloat), newNormals.data());

    glBindVertexArray(0);
}

void AnimatedMesh::submitBones(const std::vector<std::shared_ptr<Bone>>& bones, glm::mat4 objectModel, float exponent) {
    _bones = bones;

    for (unsigned i = 0; i < vertices.size(); i += 3) {
        glm::vec4 v{vertices[i],vertices[i+1],vertices[i+2],1};
        glm::vec3 vert = objectModel*v;
        glm::vec4 weights;
        glm::ivec4 boneIds{-1};

        float minVal = MAXFLOAT;
        unsigned minInd = 0;

        unsigned nbWeights = 0;

        for(const auto & bone : bones) {
            float dist = 1.f/std::pow(bone->getDistanceFrom(vert),exponent);

            if(nbWeights < 4) {
                weights[nbWeights] = dist;
                boneIds[nbWeights] = bone->id();
                ++nbWeights;

                if(dist < minVal) {
                    minVal = dist;
                    minInd = nbWeights;
                }

            } else if(dist > minVal) {
                weights[minInd] = dist;
                boneIds[minInd] = bone->id();

                minVal = weights[0];
                minInd = 0;
                for(unsigned j=1; j<4; ++j) {
                    if(weights[j] < minVal) {
                        minVal = weights[j];
                        minInd = j;
                    }
                }

            }
        }

        float sum = weights.x + weights.y + weights.z + weights.w;
        weights /= sum;

        _bonesWeights.emplace_back(weights.x);
        _bonesWeights.emplace_back(weights.y);
        _bonesWeights.emplace_back(weights.z);
        _bonesWeights.emplace_back(weights.w);
        _bonesIDs.emplace_back(boneIds.x);
        _bonesIDs.emplace_back(boneIds.y);
        _bonesIDs.emplace_back(boneIds.z);
        _bonesIDs.emplace_back(boneIds.w);
    }

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _bbo);
    glBufferData(GL_ARRAY_BUFFER, _bonesIDs.size() * sizeof(GLint), _bonesIDs.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(2, 4, GL_INT, 4 * sizeof(GLint), (GLvoid *) 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, _wbo);
    glBufferData(GL_ARRAY_BUFFER, _bonesWeights.size() * sizeof(GLfloat), _bonesWeights.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void AnimatedMesh::reset() {
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, normals.size() * sizeof(GLfloat), normals.data());

    glBindVertexArray(0);
}

