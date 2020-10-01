#ifndef HENJIN_MESH_H
#define HENJIN_MESH_H

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

class Mesh {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLuint> indices;

    void addQuad(unsigned a, unsigned b, unsigned c, unsigned d);
    void addTri(unsigned a, unsigned b, unsigned c);
    void addIndex(unsigned a);
    void addVertex(float x, float y, float z);
    void addVertex(const glm::vec3 &v);
    void addNormal(float x, float y, float z);
    void addNormal(const glm::vec3 &v);

    unsigned nbTriangles() const;
    unsigned nbVertices() const;
    unsigned nbNormals() const;
    float volume() const;

    void load();

    void draw(GLenum mode = GL_TRIANGLES) const;

    void clear();

    ~Mesh();

private :
    GLuint _vao;
    GLuint _vbo;
    GLuint _nbo;
    GLuint _ebo;

};

#endif //HENJIN_MESH_H
