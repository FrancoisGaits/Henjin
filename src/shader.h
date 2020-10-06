#ifndef HENJIN_SHADER_H
#define HENJIN_SHADER_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#define GL_GLEXT_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>

#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {

public:
    explicit Shader(bool shadows = false);

    void use();

    ~Shader();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4fv(const std::string &name, const glm::mat4 &value) const;

private:
    unsigned ID;
};

#endif //HENJIN_SHADER_H
