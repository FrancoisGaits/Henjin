#ifndef HENJIN_ISOSURFACE_H
#define HENJIN_ISOSURFACE_H

#include <functional>
#include "marchingcube.h"
#include "shape.h"
#include "src/Object/metaball.h"

using ImplicitFunction = std::function<float(glm::vec3)>;

class IsoSurface : public Shape {
public :
    explicit IsoSurface(ImplicitFunction func, glm::vec3 start, float cellSize, glm::vec3 position={0, 0, 0}, glm::vec3 color={1, 1, 1});
    explicit IsoSurface(MetaBalls mb, glm::vec3 start, float cellSize, glm::vec3 position={0, 0, 0}, glm::vec3 color={1, 1, 1});

private :
    ImplicitFunction _func;
    float _cellSize;
    unsigned _indexOffset;
    glm::vec3 _start;
    int _nbX;
    int _nbY;
    int _nbZ;

    const float _epsilon = 0.0001f;

    void fillGrid(std::vector<float>& grid, unsigned z);
    void gridsToMesh(std::vector<float>& firstGrid, std::vector<float>& secondGrid, unsigned z);
    glm::vec3 estimateNormal(glm::vec3 position);


    void draw() const override;
    void init(ImplicitFunction func, glm::vec3 start, float cellSize);

};


#endif //HENJIN_ISOSURFACE_H
