#ifndef SHAPES_H
#define SHAPES_H
#include "Mesh.h"

namespace Shapes {
Mesh createCube(float size);
Mesh createPlane(int width_n, int height_n, float edge_width, float edge_height);
Mesh createQuad();
}

#endif
