#include "line.h"


Line::Line(const std::vector<glm::vec3>& points, bool line, glm::vec3 color) : _line{line}, _color{color} {
    _model = glm::mat4(1.f);

    unsigned i = 0;
    for(auto point : points) {
        mesh.addVertex(point);
        mesh.addNormal(point);
        mesh.addIndex(i++);
    }



    mesh.load();
}

const glm::vec3 &Line::color() const {
    return _color;
}

const glm::mat4 &Line::model() const {
    return _model;
}

void Line::translate(glm::vec3 vec) {
    _model = glm::translate(_model, vec);
}

void Line::draw() const {
    mesh.draw(_line ? GL_LINE_STRIP : GL_POINTS);
}
