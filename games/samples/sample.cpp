#include "Engine.h"


/*
 *  CONTROLLER BEHAVIORS
 */
class FlashLightController : public Component {
    public:
        void update() override {
            SpotLight* sl = getGameObject()->getComponent<SpotLight>();
            Transform* t = getGameObject()->getComponent<Transform>();
            sl->setDirection(t->getWorldRotation() * glm::vec3(1.0f, 0.0f, 0.0));
        }
};

void printvec(glm::vec3 vec) {
    std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

class PlayerController : public Component {
    public:
        float move_speed_;
        float look_speed_;
        float yaw_ = 0;
        float pitch_ = 0;
        glm::vec3 look_direction_{1.0f, 0.0f, 0.0f};
        PlayerController(float move_speed = 3.0f, float look_speed = 10.0f) 
        : move_speed_(move_speed), look_speed_(look_speed) {}
        void update() {
            KeyboardHandler* kh = getGameObject()->getGame()->getKeyboardHandler();
            MouseHandler* mh = getGameObject()->getGame()->getMouseHandler();
            Transform* Pt = getGameObject()->getParent()->getComponent<Transform>();
            Transform* Lt = getGameObject()->getComponent<Transform>();
            
            yaw_ += DeltaClock::getDeltaTime() * mh->getMouseXOffset() * look_speed_;
            pitch_ -= DeltaClock::getDeltaTime() * mh->getMouseYOffset() * look_speed_;
            if (pitch_ > 89.0f) { pitch_ = 89.0f; }
            if (pitch_ < -89.0f) { pitch_ = -89.0f; }
            glm::quat rotation =
                glm::angleAxis(glm::radians(yaw_), glm::vec3(0.0f, -1.0f, 0.0f)) *
                glm::angleAxis(glm::radians(pitch_), glm::vec3(0.0f, 0.0f, 1.0f));
            look_direction_ = rotation * glm::vec3(1.0f, 0.0f, 0.0);
            Lt->setRotation(rotation);
            glm::vec3 walk_direction = 
                glm::normalize(glm::vec3(look_direction_.x, 0.0f, look_direction_.z));
            if (kh->getKeyPressed(GLFW_KEY_W)) {
                Pt->translate(DeltaClock::getDeltaTime() * move_speed_ * walk_direction);
            }
            if (kh->getKeyPressed(GLFW_KEY_S)) {
                Pt->translate(-DeltaClock::getDeltaTime() * move_speed_ * walk_direction);
            }
            if (kh->getKeyPressed(GLFW_KEY_D)) {
                Pt->translate(DeltaClock::getDeltaTime() * glm::normalize(glm::cross(walk_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * move_speed_);
            }
            if(kh->getKeyPressed(GLFW_KEY_A)) {
                Pt->translate(-DeltaClock::getDeltaTime() * glm::normalize(glm::cross(walk_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * move_speed_);
            }
        }      
};

class PlayerFPSCamera : public Camera {
    public:
        glm::vec3 getFront() override { 
            GameObject* parent = getGameObject()->getParent();
            PlayerController* pc = parent->getComponent<PlayerController>();
            return pc->look_direction_;
        }
        glm::vec3 getPosition() override {
            Transform* t = getGameObject()->getComponent<Transform>();
            return t->getWorldPosition();
        }

};
/*
 *  PREFAB DEFINITIONS
 */
class FlashLight : public GameObject {
    public:
        FlashLight(Game* game) : GameObject(game) {
            addComponent<Transform>();
            addComponent<FlashLightController>();
            addComponent<SpotLight>();
        }
};

class PlayerObject : public GameObject {
    public:
        PlayerObject(Game* game) : GameObject(game) {
            addComponent<Transform>();
        }
};
class SimpleObject : public GameObject {
    public:
        SimpleObject(Game* game, std::string model_name, std::string shader_name) : GameObject(game) {
            addComponent<Transform>();
            ModelID mod_id = AssetManager::findAssetID<Model>(model_name);
            ShaderID shader_id = AssetManager::findAssetID<Shader>(shader_name);
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

class MountObject : public GameObject {
    public:
        MountObject(Game* game) : GameObject(game) {
            addComponent<Transform>();
        }
};

/*
 * ASSET LOADING
 */
void genShaders() {
    Shader phong_shader = ShaderLoader::loadShaderFromFile(
        PathUtils::shaderDir / "phong/phong.vert",
        PathUtils::shaderDir / "phong/phong.frag");
    AssetManager::registerAsset<Shader>("phong_shader", phong_shader);

    Shader flat_shader = ShaderLoader::loadShaderFromFile(
        PathUtils::shaderDir / "flat/flat.vert",
        PathUtils::shaderDir / "flat/flat.frag");
    AssetManager::registerAsset<Shader>("flat_shader", flat_shader);
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

void getFloorAssets() {
    TextureID diffuse = AssetManager::loadTexture(
            PathUtils::textureDir / "grass15.png");

    float size = 1000;
    float tile_size = 2;
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

/*
 * SCENE SETUP / MAIN EXECUTION
 */
int main() {
    int window_width = 1000;
    int window_height = 1000;
    Game game(window_width, window_height);

    genContainerAssets();
    getFloorAssets();
    genShaders();

    // Container instance creation
    SimpleObject container(&game, "container_model", "phong_shader");
    game.addGameObject(&container);
    Transform* t = container.getComponent<Transform>();
    t->scale(glm::vec3(2.0f, 2.0f, 2.0f));
    t->translate(glm::vec3(5.0f, 0.0f, 0.0f));

    // Floor instance creation
    SimpleObject floor_object(&game, "floor_model", "phong_shader");
    game.addGameObject(&floor_object);
    t = floor_object.getComponent<Transform>();
    t->translate(glm::vec3(0.0f, -1.0f, 0.0f));
    
    // Backpack instance creation (Uses chosen obj file)
    ModelObject backpack(&game, PathUtils::objectDir / "backpack/backpack.obj");
    game.addGameObject(&backpack);
    t = backpack.getComponent<Transform>();
    t->translate(glm::vec3(2.0f, 0.0f, 5.0f));
    t->rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    // Player/Camera/Flashlight setup
    float player_height = 4.0f;
    PlayerObject player(&game);
    game.addGameObject(&player);

    MountObject camera(&game);
    game.addGameObject(&camera);
    game.setCamera(camera.addComponent<Camera>());
    camera.addComponent<PlayerController>();
    player.addChild(&camera);
    t = camera.getComponent<Transform>();
    t->translate(glm::vec3(0.0f, player_height, 0.0f));

    FlashLight flashlight(&game);
    game.addGameObject(&flashlight);
    game.addLightSource(flashlight.getComponent<SpotLight>());
    camera.addChild(&flashlight);
    t = flashlight.getComponent<Transform>();
    t->translate(glm::vec3(0.0f, -0.5f, 0.5f));

    GameObject sunlight(&game);
    game.addLightSource(sunlight.addComponent<DirectionalLight>());
    game.addGameObject(&sunlight);

    game.Loop();
}
