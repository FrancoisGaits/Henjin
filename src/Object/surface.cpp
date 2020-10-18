#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "surface.h"


Surface::Surface(const std::vector<std::vector<glm::vec3>> &points, glm::vec3 color, float offset, float metalness, float roughness) : _metalness(metalness), _roughness(roughness){
    _color = glm::pow(color,glm::vec3(2.2f));
    _model = glm::mat4(1.f);

    init(points,color,offset);

}


Surface::Surface(const BSplineTensor &tensor, float pas, glm::vec3 color, float offset, float metalness, float roughness) : _metalness(metalness), _roughness(roughness) {
    _color = glm::pow(color,glm::vec3(2.2f));
    _model = glm::mat4(1.f);

    std::vector<std::vector<glm::vec3>> pointspoints;
    std::vector<glm::vec3> points;

    for(float u = tensor.startUInterval(); u < tensor.endUInterval(); u += pas) {
        for(float v = tensor.startVInterval(); v < tensor.endVInterval(); v += pas) {

            glm::vec3 p = tensor.eval(u,v);
            points.emplace_back(p);
        }
        pointspoints.emplace_back(points);
        points.clear();
    }

    std::cout << std::endl;



    init(pointspoints, color, offset);
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

void Surface::init(const std::vector<std::vector<glm::vec3>> &points, glm::vec3 color, float offset) {
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> vertices;

    std::vector<Quad> savedQuads;

    for(const auto& line : points) {
        for(const auto& p : line) {
            mesh.addVertex(p);
            vertices.emplace_back(p);
            normals.emplace_back(glm::vec3(0));
        }
    }
    if(offset > 0.f) {
        for (const auto &line : points) {
            for (const auto &p : line) {
                mesh.addVertex(p - glm::vec3(0, offset, 0));
                vertices.emplace_back(p - glm::vec3(0, offset, 0));
                normals.emplace_back(glm::vec3(0));
            }
        }
    }

    unsigned n = points.back().size();
    unsigned nb_points = n*points.size();

    for(unsigned x = 0; x<n-1; ++x) {
        for(unsigned y = 0; y<points.size()-1; ++y) {
            unsigned a = y*n + x;
            unsigned b = y*n + x+1;
            unsigned c = (y+1)*n + x;
            unsigned d = (y+1)*n + x+1;
            mesh.addQuad(a,b,c,d);

            if(offset > 0.f) {
                if (x == 0) {
//                    mesh.addQuad(a, c, a + nb_points, c + nb_points);
                    savedQuads.emplace_back(Quad{a,c,a+nb_points, c+nb_points});
                } else if (x == n - 2) {
//                    mesh.addQuad(d, b, d + nb_points, b + nb_points);
                    savedQuads.emplace_back(Quad{d,b,d+nb_points, b+nb_points});
                }

                if (y == 0) {
//                    mesh.addQuad(a, b, a + nb_points, b + nb_points);
                    savedQuads.emplace_back(Quad{a,b,a+nb_points, b+nb_points});
                } else if (y == points.size() - 2) {
//                    mesh.addQuad(d, c, d + nb_points, c + nb_points);
                    savedQuads.emplace_back(Quad{d,c,d+nb_points, c+nb_points});
                }
            }



            glm::vec3 normalLower = glm::cross(vertices[a]-vertices[d],vertices[c]-vertices[d]);
            glm::vec3 normalUpper = glm::cross(vertices[d]-vertices[a],vertices[b]-vertices[a]);


            normals[a] += normalLower;
            normals[c] += normalLower;
            normals[d] += normalLower;

            normals[a] += normalUpper;
            normals[d] += normalUpper;
            normals[b] += normalUpper;
        }
    }


    if (offset > 0.f) {
        for (unsigned x = 0; x < n - 1; ++x) {
            for (unsigned y = 0; y < points.size() - 1; ++y) {
                unsigned a = y * n + x + nb_points;
                unsigned b = y * n + x + 1 + nb_points;
                unsigned c = (y + 1) * n + x + nb_points;
                unsigned d = (y + 1) * n + x + 1 + nb_points;
                mesh.addQuad(a, b, c, d);

                glm::vec3 normalLower = glm::cross(vertices[a] - vertices[d], vertices[c] - vertices[d]);
                glm::vec3 normalUpper = glm::cross(vertices[d]-vertices[a],vertices[b]-vertices[a]);


                normals[a] -= normalLower;
                normals[c] -= normalLower;
                normals[d] -= normalLower;

                normals[a] -= normalUpper;
                normals[d] -= normalUpper;
                normals[b] -= normalUpper;
            }
        }
    }

    for(const auto & p : normals) {
        mesh.addNormal(p);
    }

    unsigned i = mesh.nbVertices();
    for(const auto & quad : savedQuads) {
        glm::vec3 normal = glm::cross(vertices[quad.c]-vertices[quad.a],vertices[quad.d]-vertices[quad.a]);
        normal.z = -normal.z;

        mesh.addNormal(normal);
        mesh.addNormal(normal);
        mesh.addNormal(normal);
        mesh.addNormal(normal);

        mesh.addVertex(vertices[quad.a]);
        mesh.addVertex(vertices[quad.b]);
        mesh.addVertex(vertices[quad.c]);
        mesh.addVertex(vertices[quad.d]);

        mesh.addQuad(i,i+1,i+2,i+3);
        i += 4;
    }


    mesh.load();
}



void Surface::setColor(const glm::vec3 &newColor) {
    _color = glm::pow(newColor,glm::vec3(2.2f));
}

void Surface::setMetalness(float metalness) {
    _metalness = metalness;
}

void Surface::setRoughness(float roughness) {
    _roughness = roughness;
}



