#ifndef HENJIN_RENDERQUAD_H
#define HENJIN_RENDERQUAD_H

#define GL_GLEXT_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>



class RenderQuad {
public:
    RenderQuad();

    void draw();

private:
    GLuint _vao;
    GLuint _vbo;
};

#endif //HENJIN_RENDERQUAD_H
