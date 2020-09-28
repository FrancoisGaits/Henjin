#include "model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(const std::string& name, glm::vec3 position, glm::vec3 color, float scale, float div) : Shape(position, color, scale), _name{name}{
    _mesh = loadMesh(name, div);

    _mesh.load();
}

Mesh Model::loadMesh(const std::string &name, float div) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    Mesh tmpMesh;

    std::string path = "../models/" + name;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());

    if (!ret) {
        std::cerr << "Couldn't read object " << path << std::endl;
        return tmpMesh;
    } else if (shapes.size() != 1) {
        std::cerr << "Too many shapes, the mesh can only be triangles" << std::endl;
        return tmpMesh;
    }

    //Lecture typique avec duplication des sommets pour chaque faces, non necessaire mais standard
    for (const auto & shape : shapes) {
        size_t index_offset = 0;
        int ind = 0;
        for (long unsigned f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            unsigned fv = shape.mesh.num_face_vertices[f];
            for (unsigned v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
                tinyobj::real_t nx = attrib.normals[3 * idx.normal_index];
                tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

                //On pourrait bien sur utiliser la matrice model pour scale l'objet, mais les tailles varient
                // énormément d'un modèle à l'autre, div permet de "normaliser" la taille
                tmpMesh.addVertex(vx / div, vy / div, vz / div);
                tmpMesh.addNormal(nx, ny, nz);

            }
            tmpMesh.addTri(ind, ind+1, ind+2);
            ind += 3;
            index_offset += fv;
        }
    }

    return tmpMesh;
}

void Model::draw() const {
    _mesh.draw();
}
