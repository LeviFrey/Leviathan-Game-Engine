#pragma once
#include <vector>
#include "Mesh.h"
#include "Material.h"


struct RenderPart {
    Mesh mesh_;
    Material material_;
};

struct Model {
    std::vector<RenderPart> parts_;
    void addPart(RenderPart part) { parts_.push_back(part); }
    void addPart(Mesh mesh, Material material) { parts_.push_back({mesh, material}); } 
};


