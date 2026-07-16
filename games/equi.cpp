#include "Engine.h"
#include <iostream>
#include <filesystem>

int main() {
    Game game(1500, 900);

    // Textures
    std::filesystem::path dir = PathUtils::textureDir / "sky";
    Texture skyboxTex = TextureLoader::loadCubemapFromFile({
        dir / "right.jpg",
        dir / "left.jpg",
        dir / "top.jpg",
        dir / "bottom.jpg",
        dir / "front.jpg",
        dir / "back.jpg"
    });

    TextureID cubemap = AssetManager::storeCubemap(skyboxTex);
            

    game.setSkybox(cubemap);
    game.Loop();
}


