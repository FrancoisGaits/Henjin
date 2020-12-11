#ifndef HENJIN_BONE_H
#define HENJIN_BONE_H

#include <glm/glm.hpp>
#include <memory>
#include "glm/gtx/transform.hpp"
#include "glm/gtx/norm.hpp"
#include "src/Object/mesh.h"

class Bone  {
public:
    explicit Bone(glm::vec3 position, float length=1.f, glm::vec3 rotation=glm::vec3{0}, glm::mat4 parentTranslation=glm::mat4{1}, glm::mat4 parentRotation = glm::mat4{1});

    void draw() const;

    const glm::vec3 &color() const;
    const glm::mat4 &model() const;
    const glm::mat4 &translation() const;
    const glm::mat4 &rotation() const;

    void translate(glm::vec3 vec);

    void rotate(glm::vec3 rot);

    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    glm::mat4 getTransform();
    const glm::vec3& origStart();
    const glm::vec3& origEnd();

    void registerChild(const std::shared_ptr<Bone>& bone);
    void setParentTransform(glm::mat4 translation, glm::mat4 rotation);

    float getDistanceFrom(glm::vec3 point);

private:

    float _lenght;

    glm::mat4 _totalModel;

    glm::mat4 _inverseInitModel;
    glm::vec3 _origStart;
    glm::vec3 _origEnd;

    glm::mat4 _translation;
    glm::mat4 _rotation;

    Mesh _mesh;

    glm::vec3 _color;


    glm::mat4 _parentTranslation;
    glm::mat4 _parentRotation;
    std::vector<std::shared_ptr<Bone>> _children;

    void updateModel();


};


#endif //HENJIN_BONE_H
