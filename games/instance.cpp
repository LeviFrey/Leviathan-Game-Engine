#include "Engine.h"
#include "PlayerController.h"


class VisualObject : public GameObject {
    public:
        VisualObject(Game* game, std::string model, std::string shader) : GameObject(game) {
            addComponent<Transform>();
            ModelID mod_id = AssetManager::findAssetID<Model>(model);
            ShaderID shad_id = AssetManager::findAssetID<Shader>(shader);
            addComponent<Renderer>(mod_id, shad_id);
        }
};

class InvisibleObject : public GameObject {
    public:
        InvisibleObject(Game* game) : GameObject(game) {
            addComponent<Transform>();
        }
};

void genShaders() {
    Shader flat = ShaderLoader::loadShaderFromFile(
            PathUtils::shaderDir / "flat/flat.vert",
            PathUtils::shaderDir / "flat/flat.frag");
    AssetManager::registerAsset<Shader>("flat_shader", flat);
}

void genFloorAssets() {
    TextureID diffuse = AssetManager::loadTexture(
        PathUtils::textureDir / "grass15.png");

    float size = 1000;
    float tile_size = 5;
    Material mat;
    mat.diffuse_ = diffuse;
    mat.shininess_ = 32.0f;
    mat.tesselationRate_ = size/tile_size;
    MaterialID mat_id = AssetManager::storeAsset<Material>(mat);

    Mesh mesh = Shapes::createPlane(1, 1, size, size);
    MeshID mesh_id = AssetManager::storeAsset<Mesh>(mesh);

    Model model({{{mesh_id, mat_id}}});
    AssetManager::registerAsset<Model>("floor_model", model);
}

int main() {
    float win_width = 1300;
    float win_height = 1000;
    Game game(1300, 1000);
    
    genShaders();
    genFloorAssets();
    VisualObject floor(&game, "floor_model", "flat_shader");
    game.addGameObject(&floor);

    InvisibleObject player(&game);
    InvisibleObject head(&game);
    player.addChild(&head);
    head.addComponent<PlayerController>();
    game.setCamera(head.addComponent<Camera>());
    game.addGameObject(&player);
    game.addGameObject(&head);
    game.setCamera(head.addComponent<Camera>());
    Transform* t = head.getComponent<Transform>();
    t->translate(glm::vec3(0.0f, 3.0f, 0.0f));

    game.Loop();
}
