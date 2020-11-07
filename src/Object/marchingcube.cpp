#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "marchingcube.h"

int MarchingCube::polygonise(gridCell &grid, Mesh &mesh, unsigned indexOffset) {
    unsigned cubeIndex;

    glm::vec3 VertexList[12];
    glm::vec3 NewVertexList[12];
    char LocalRemap[12];

    //Determine the index into the edge table which
    //tells us which vertices are inside of the surface

    cubeIndex = 0;
    if (grid.val[0] < 0) cubeIndex |= 1;
    if (grid.val[1] < 0) cubeIndex |= 2;
    if (grid.val[2] < 0) cubeIndex |= 4;
    if (grid.val[3] < 0) cubeIndex |= 8;
    if (grid.val[4] < 0) cubeIndex |= 16;
    if (grid.val[5] < 0) cubeIndex |= 32;
    if (grid.val[6] < 0) cubeIndex |= 64;
    if (grid.val[7] < 0) cubeIndex |= 128;

    //Cube is entirely in/out of the surface
    if (edgeTable[cubeIndex] == 0)
        return(0);

    //Find the vertices where the surface intersects the cube
    if (edgeTable[cubeIndex] & 1)
        VertexList[0] =
                vertexInterp(grid.points[0],grid.points[1],grid.val[0],grid.val[1]);
    if (edgeTable[cubeIndex] & 2)
        VertexList[1] =
                vertexInterp(grid.points[1],grid.points[2],grid.val[1],grid.val[2]);
    if (edgeTable[cubeIndex] & 4)
        VertexList[2] =
                vertexInterp(grid.points[2],grid.points[3],grid.val[2],grid.val[3]);
    if (edgeTable[cubeIndex] & 8)
        VertexList[3] =
                vertexInterp(grid.points[3],grid.points[0],grid.val[3],grid.val[0]);
    if (edgeTable[cubeIndex] & 16)
        VertexList[4] =
                vertexInterp(grid.points[4],grid.points[5],grid.val[4],grid.val[5]);
    if (edgeTable[cubeIndex] & 32)
        VertexList[5] =
                vertexInterp(grid.points[5],grid.points[6],grid.val[5],grid.val[6]);
    if (edgeTable[cubeIndex] & 64)
        VertexList[6] =
                vertexInterp(grid.points[6],grid.points[7],grid.val[6],grid.val[7]);
    if (edgeTable[cubeIndex] & 128)
        VertexList[7] =
                vertexInterp(grid.points[7],grid.points[4],grid.val[7],grid.val[4]);
    if (edgeTable[cubeIndex] & 256)
        VertexList[8] =
                vertexInterp(grid.points[0],grid.points[4],grid.val[0],grid.val[4]);
    if (edgeTable[cubeIndex] & 512)
        VertexList[9] =
                vertexInterp(grid.points[1],grid.points[5],grid.val[1],grid.val[5]);
    if (edgeTable[cubeIndex] & 1024)
        VertexList[10] =
                vertexInterp(grid.points[2],grid.points[6],grid.val[2],grid.val[6]);
    if (edgeTable[cubeIndex] & 2048)
        VertexList[11] =
                vertexInterp(grid.points[3],grid.points[7],grid.val[3],grid.val[7]);

    char NewVertexCount=0;
    for (int i=0;i<12;i++)
        LocalRemap[i] = -1;

    for (int i=0;triTable[cubeIndex][i]!=-1;i++)
    {
        if(LocalRemap[triTable[cubeIndex][i]] == -1)
        {
            NewVertexList[NewVertexCount] = VertexList[triTable[cubeIndex][i]];
            LocalRemap[triTable[cubeIndex][i]] = NewVertexCount;
            NewVertexCount++;
        }
    }

    for (int i=0;i<NewVertexCount;i++) {
        mesh.addVertex(NewVertexList[i]);
    }

    for (unsigned i=0;triTable[cubeIndex][i]!=-1;i+=3) {
        unsigned a = LocalRemap[triTable[cubeIndex][i+0]] + indexOffset;
        unsigned b = LocalRemap[triTable[cubeIndex][i+1]] + indexOffset;
        unsigned c = LocalRemap[triTable[cubeIndex][i+2]] + indexOffset;
        mesh.addTri(a,b,c);
    }

    return NewVertexCount;
}

glm::vec3 MarchingCube::vertexInterp(const glm::vec3 &p1, const glm::vec3 &p2, float val1, float val2) {
    return (p1 + (-val1 / (val2 - val1)) * (p2 - p1));
}
