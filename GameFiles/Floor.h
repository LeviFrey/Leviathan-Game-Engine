#pragma once
#include "Engine.h"

class Floor : public GameObject {
    public:
    Floor(float tile_size, unsigned int num_tiles, AssetManager* assets) {
        
        // Access global state texture
        Texture* texture = assets->loadAsset<Texture>(
            {PathUtils::textureDir / "grass_floor.png"}
        );
        
        // Create custom model
        Model model;
        PhongMaterial* material = model.addUniqueMaterial<PhongMaterial>();
        material->diffuse_ = texture;
        material->tesselation_rate_ = num_tiles;

        float total_size = tile_size * num_tiles;
        Mesh* mesh = model.addUniqueMesh(
            Shapes::createPlane(
                1, 1,
                total_size, total_size
            )
        );

        model.addPart(mesh, material);
        
        // Add Components
        Model* model_ptr = assets->storeAsset<Model>(std::move(model));
        Shader* shader = assets->loadAsset<Shader>({
            PathUtils::shaderDir / "flat/flat.vert",
            PathUtils::shaderDir / "flat/flat.frag"
        });

        addComponent<Renderer>(model_ptr, shader);
        addComponent<Transform>();    
    }
};
