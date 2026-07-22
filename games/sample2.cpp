#include "Engine.h"

int main() {
    int window_width = 1000;
    int window_height = 1000;
    Game game(window_width, window_height);

    ShaderID shader_id = AssetManager::loadShader(
        PathUtils::shaderDir / "flat/flat.vert",
        PathUtils::shaderDir / "flat/flat.frag");

    TextureID tex_id = AssetManager::loadTexture(
            PathUtils::textureDir / "container2.jpg");
    
    Material mat;
    mat.diffuse_ = tex_id;
    mat.shininess_ = 32.0f;



    AssetManager::registerAsset<Material>("container_material", mat);
    MaterialID mat_id = AssetManager::getAssetFromName<Material>("container_material");

    Model model({{{AssetManager::defaultMeshes().cube_, mat_id}}});

    ModelID model_id = AssetManager::storeAsset<Model>(model);
    
    GameObject container(&game); 
    Transform* t = container.addComponent<Transform>();
    Renderer* r = container.addComponent<Renderer>(model_id, shader_id);
    game.addGameObject(&container);

    GameObject camera(&game);
    t = camera.addComponent<Transform>();
    FreeCamera* c = camera.addComponent<FreeCamera>();
    c->setAspectRatio((float)window_width/(float)window_height);
    game.setCamera(c);
    game.addGameObject(&camera);

    game.Loop();
}
