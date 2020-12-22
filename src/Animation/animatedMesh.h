#ifndef HENJIN_ANIMATEDMESH_H
#define HENJIN_ANIMATEDMESH_H


#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#define GL_GLEXT_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>
#include "glm/glm.hpp"

#endif

#include <vector>
#include <memory>
#include <unordered_map>

#include "bone.h"

class AnimatedMesh {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLuint> indices;


    void addQuad(unsigned a, unsigned b, unsigned c, unsigned d);
    void addTri(unsigned a, unsigned b, unsigned c);
    void addVertex(float x, float y, float z);
    void addVertex(glm::vec3 &v);
    void addNormal(float x, float y, float z);
    void addNormal(glm::vec3 &n);

    unsigned nbTriangles() const;
    unsigned nbVertices() const;
    unsigned nbNormals() const;

    void update(glm::mat4 meshModel);
    void submitBones(const std::vector<std::shared_ptr<Bone>>& bones, glm::mat4 objectModel, float exponent=4.f);

    void load();

    void draw(GLenum mode=GL_TRIANGLES) const;

    void clear();
    void reset();

    ~AnimatedMesh();

private :
    GLuint _vao;
    GLuint _vbo;
    GLuint _nbo;
    GLuint _ebo;
    GLuint _bbo;
    GLuint _wbo;

    std::vector<std::shared_ptr<Bone>> _bones;
    std::vector<int> _bonesIDs;
    std::vector<float> _bonesWeights;
};



#endif //HENJIN_ANIMATEDMESH_H
