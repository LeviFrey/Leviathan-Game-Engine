#pragma once
#include <vector>
#include "Mesh.h"
#include "Material.h"


struct RenderPart {
    Mesh mesh_;
    Material material_;
};

class Model {
    public:
        const RenderPart& getRenderPart(int index_id) const { 
            return parts_[index_id]; 
        }

        const std::vector<RenderPart>& getParts() const { 
            return parts_; 
        }

        void addPart(RenderPart part) { 
            parts_.push_back(part); 
        }

        void addPart(Mesh mesh, Material material) { 
            parts_.push_back({mesh, material}); 
        } 
    private:
        std::vector<RenderPart> parts_;
};


