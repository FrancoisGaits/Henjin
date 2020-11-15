#ifndef HENJIN_SKYBOX_H
#define HENJIN_SKYBOX_H

#define GL_GLEXT_PROTOTYPES 1

#include <GL/gl.h>
#include "lodepng.h"
#include "shape.h"
#include <GL/glext.h>


class Skybox {
public:
    Skybox();

    void load();
    void bind(GLenum textureUnit);

    void draw();

private:
    GLuint _texture;

    std::string _fileNames[6] = {
            "right.png",
            "left.png",
            "top.png",
            "bottom.png",
            "front.png",
            "back.png"
    };

    GLenum _typeList[6] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    GLuint _vao;
    GLuint _vbo;


};


#endif //HENJIN_SKYBOX_H
