#include <iostream>
#include "surface.h"


Surface::Surface(const std::vector<std::vector<glm::vec3>> &points, glm::vec3 color) : _color{color}{
    _model = glm::mat4(1.f);

    std::vector<std::pair<glm::vec3,unsigned>> normals;
    std::vector<glm::vec3> vertices;

    for(const auto& line : points) {
        for(const auto& p : line) {
            mesh.addVertex(p);
            vertices.emplace_back(p);
            normals.emplace_back(std::make_pair(glm::vec3(0),0));
        }
    }

    unsigned n = points.back().size();

    std::cout << mesh.nbVertices() << std::endl;
    for(unsigned x = 0; x<n-1; ++x) {
        for(unsigned y = 0; y<points.size()-1; ++y) {
            unsigned a = y*n + x;
            unsigned b = y*n + x+1;
            unsigned c = (y+1)*n + x;
            unsigned d = (y+1)*n + x+1;
            mesh.addQuad(a,b,c,d);

            glm::vec3 normalLower = glm::cross(vertices[a]-vertices[d],vertices[c]-vertices[d]);
            glm::vec3 normalUpper = glm::cross(vertices[a]-vertices[b],vertices[c]-vertices[b]);


            normals[a].second++;
            normals[a].first = normals[a].first + ((normalLower-normals[a].first)/static_cast<float>(normals[a].second));
            normals[c].second++;
            normals[c].first = normals[c].first + ((normalLower-normals[c].first)/static_cast<float>(normals[c].second));
            normals[d].second++;
            normals[d].first = normals[d].first + ((normalLower-normals[d].first)/static_cast<float>(normals[d].second));

            normals[a].second++;
            normals[a].first = normals[a].first + ((normalUpper-normals[a].first)/static_cast<float>(normals[a].second));
            normals[c].second++;
            normals[c].first = normals[c].first + ((normalUpper-normals[c].first)/static_cast<float>(normals[c].second));
            normals[b].second++;
            normals[b].first = normals[b].first + ((normalUpper-normals[b].first)/static_cast<float>(normals[b].second));
        }
    }

    for(const auto & p : normals) {
        mesh.addNormal(p.first);
    }

    mesh.load();

}


const glm::vec3 &Surface::color() const {
    return _color;
}

const glm::mat4 &Surface::model() const {
    return _model;
}

void Surface::draw() const {
    mesh.draw();
}

