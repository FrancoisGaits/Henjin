#include "shader.h"

Shader::Shader(bool shadows) {
    std::string vertexPath(shadows ? "../shaders/shadows.vs" : "../shaders/shader.vs");
    std::string fragmentPath(shadows ? "../shaders/shadows.fs" : "../shaders/shader.fs");

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    GLchar log[512];
    GLint success = 0;
    GLuint vertexshader, fragmentshader;

    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexshader);



    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexshader, 512, nullptr, log);
        std::cerr << "Echec de compilation du vertex shader : \n" << log << std::endl;
    }

    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentshader);

    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentshader, 512, nullptr, log);
        std::cerr << "Echec de compilation du fragment shader : \n" << log << std::endl;
    }

    ID = glCreateProgram();

    glAttachShader(ID, vertexshader);
    glAttachShader(ID, fragmentshader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, nullptr, log);
        std::cerr << "Echec de link : \n" << log << std::endl;
    }

    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
}

void Shader::setBool(const std::string &name, bool value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc >= 0)
        glUniform1i(loc, (int) value);
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}

void Shader::setInt(const std::string &name, int value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc >= 0)
        glUniform1i(loc, value);
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}

void Shader::setFloat(const std::string &name, float value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc >= 0)
        glUniform1f(loc, value);
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}

void Shader::setMat4fv(const std::string &name, const glm::mat4 &value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc >= 0)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc >= 0)
        glUniform3f(loc, value.x, value.y, value.z);
    else
        std::cerr << "Unable to locate '" << name << "' Uniform in shader : " << ID << std::endl;
}

void Shader::use() {
    glUseProgram(ID);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}



