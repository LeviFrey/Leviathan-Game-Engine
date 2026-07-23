#include "Engine.h"

class FlashLightController : public Component {
    public:
        void update() override {
            SpotLight* sl = getGameObject()->getComponent<SpotLight>();
            Transform* t = getGameObject()->getComponent<Transform>();
            sl->setDirection(t->getWorldRotation() * glm::vec3(1.0f, 0.0f, 0.0));
        }
};

class FlashLight : public GameObject {
    public:
        FlashLight(Game* game) : GameObject(game) {
            addComponent<Transform>();
            addComponent<FlashLightController>();
            addComponent<SpotLight>();
        }
};

class Container : public GameObject {
    public:
        Container(Game* game) : GameObject(game) {
            addComponent<Transform>();
            ModelID mod_id = AssetManager::findAssetID<Model>("container_model");
            ShaderID shader_id = AssetManager::findAssetID<Shader>("phong_shader");
            addComponent<Renderer>(mod_id, shader_id);
        }
};

class ModelObject : public GameObject {
    public:
        ModelObject(Game* game, std::filesystem::path obj_path) : GameObject(game) {
            addComponent<Transform>();
            ModelID mod_id = AssetManager::loadModel(obj_path);
            ShaderID shader_id = AssetManager::findAssetID<Shader>("phong_shader");
            addComponent<Renderer>(mod_id, shader_id);
        }
};


void genShaders() {
    Shader shader = ShaderLoader::loadShaderFromFile(
        PathUtils::shaderDir / "phong/phong.vert",
        PathUtils::shaderDir / "phong/phong.frag");
    AssetManager::registerAsset<Shader>("phong_shader", shader);
}

void genContainerAssets() {
    TextureID diffuse = AssetManager::loadTexture(
            PathUtils::textureDir / "container2.jpg");
    TextureID specular = AssetManager::loadTexture(
            PathUtils::textureDir / "container2_specular.jpg");
    
    Material mat;
    mat.diffuse_ = diffuse;
    mat.specular_ = specular;
    mat.shininess_ = 32.0f;
    MaterialID mat_id = AssetManager::storeAsset<Material>(mat);

    Model model({{{AssetManager::defaultMeshes().cube_, mat_id}}}); 
    AssetManager::registerAsset<Model>("container_model", model);
}

int main() {
    int window_width = 1000;
    int window_height = 1000;
    Game game(window_width, window_height);

    genContainerAssets();
    genShaders();

    // Container instance creation
    Container container(&game);
    game.addGameObject(&container);
    Transform* t = container.getComponent<Transform>();
    t->translate(glm::vec3(5.0f, 0.0f, 0.0f));
    
    // Backpack instance creation (Uses chosen obj file)
    ModelObject backpack(&game, PathUtils::objectDir / "backpack/backpack.obj");
    game.addGameObject(&backpack);
    t = backpack.getComponent<Transform>();
    t->translate(glm::vec3(2.0f, 0.0f, 5.0f));
    
    // Camera setup
    GameObject camera(&game);
    t = camera.addComponent<Transform>();
    game.setCamera(camera.addComponent<FreeCamera>());
    game.addGameObject(&camera);
    
    // mount a flashlight holding position
    FlashLight fl(&game);
    game.addGameObject(&fl);
    camera.addChild(&fl);
    t = fl.getComponent<Transform>();
    t->translate(glm::vec3(0.0f, 0.0f, 0.5f));
    game.addLightSource(fl.getComponent<SpotLight>());

    game.Loop();
}
