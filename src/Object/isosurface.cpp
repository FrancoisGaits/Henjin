#include "isosurface.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

IsoSurface::IsoSurface(ImplicitFunction func, glm::vec3 start, float cellSize, glm::vec3 position, glm::vec3 color) : Shape(position, color, 1) {
    init(func, start, cellSize);
}


IsoSurface::IsoSurface(MetaBalls mb, glm::vec3 start, float cellSize, glm::vec3 position, glm::vec3 color) : Shape(position, color, 1) {
    ImplicitFunction func = std::function<float(glm::vec3)>(mb);
    init(func, start, cellSize);

}

void IsoSurface::fillGrid(std::vector<float> &grid, unsigned z) {
    glm::vec3 pos;
    for(unsigned x = 0; x < _nbX; ++x) {
        for(unsigned y = 0; y <_nbY; ++y) {
            pos = {_start.x + _cellSize * x, _start.y + _cellSize * y, _start.z + _cellSize * (z+1)};
            grid[x*_nbY + y] = _func(pos);
        }
    }
}

void IsoSurface::gridsToMesh(std::vector<float>& firstGrid, std::vector<float>& secondGrid, unsigned z) {

    for(unsigned x = 0; x < _nbX-1; ++x) {
        for(unsigned y = 0; y < _nbY-1; ++y) {
            gridCell g{};
            g.points[0] = {_start.x + _cellSize * (x+0), _start.y + _cellSize * (y+0), _start.z + _cellSize * (z+0)};
            g.points[1] = {_start.x + _cellSize * (x+1), _start.y + _cellSize * (y+0), _start.z + _cellSize * (z+0)};
            g.points[2] = {_start.x + _cellSize * (x+1), _start.y + _cellSize * (y+1), _start.z + _cellSize * (z+0)};
            g.points[3] = {_start.x + _cellSize * (x+0), _start.y + _cellSize * (y+1), _start.z + _cellSize * (z+0)};

            g.val[0] = firstGrid[x*_nbY+y];
            g.val[1] = firstGrid[(x+1)*_nbY+y];
            g.val[2] = firstGrid[(x+1)*_nbY+y+1];
            g.val[3] = firstGrid[x*_nbY+y+1];

            g.points[4] = {_start.x + _cellSize * (x+0), _start.y + _cellSize * (y+0), _start.z + _cellSize * (z+1)};
            g.points[5] = {_start.x + _cellSize * (x+1), _start.y + _cellSize * (y+0), _start.z + _cellSize * (z+1)};
            g.points[6] = {_start.x + _cellSize * (x+1), _start.y + _cellSize * (y+1), _start.z + _cellSize * (z+1)};
            g.points[7] = {_start.x + _cellSize * (x+0), _start.y + _cellSize * (y+1), _start.z + _cellSize * (z+1)};

            g.val[4] = secondGrid[x*_nbY+y];
            g.val[5] = secondGrid[(x+1)*_nbY+y];
            g.val[6] = secondGrid[(x+1)*_nbY+y+1];
            g.val[7] = secondGrid[x*_nbY+y+1];

            _indexOffset += MarchingCube::polygonise(g,_mesh,_indexOffset);
        }
    }
}

void IsoSurface::draw() const {
    _mesh.draw();
}

glm::vec3 IsoSurface::estimateNormal(glm::vec3 position) {
    float value = _func(position);
    glm::vec3 normal;
    normal.x = (_func(glm::vec3(position.x + _epsilon, position.y, position.z)) - value);
    normal.y = (_func(glm::vec3(position.x, position.y + _epsilon, position.z)) - value);
    normal.z = (_func(glm::vec3(position.x, position.y, position.z + _epsilon)) - value);
    normal = -glm::normalize(normal);

    return normal;
}

void IsoSurface::init(ImplicitFunction func, glm::vec3 start, float cellSize) {
    _func = std::move(func);
    _cellSize = cellSize;
    _start = start;
    _indexOffset = 0;

    glm::vec3 box = start + start;


    _nbX = static_cast<int>(std::abs(box.x)/cellSize)+1;
    _nbY = static_cast<int>(std::abs(box.y)/cellSize)+1;
    _nbZ = static_cast<int>(std::abs(box.z)/cellSize)+1;

    std::vector<float> firstGrid;
    firstGrid.resize(_nbX*_nbY);
    std::vector<float> secondGrid;
    secondGrid.resize(_nbX*_nbY);

    fillGrid(firstGrid, 0);
    for(unsigned z = 1; z < _nbZ+1; ++z) {
        fillGrid(secondGrid, z);
        gridsToMesh(firstGrid,secondGrid,z);
        std::copy(secondGrid.begin(), secondGrid.end(), firstGrid.begin());
    }

    for(unsigned i=0; i<_mesh.nbVertices(); ++i) {
        glm::vec3 vert = {_mesh.vertices[i*3],_mesh.vertices[i*3+1],_mesh.vertices[i*3+2]};
        glm::vec3 normal = estimateNormal(vert);
        _mesh.addNormal(normal);
    }

    _mesh.load();
}



