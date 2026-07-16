#include "Engine.h"
#include "../behaviors/Flashlight.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

/*
 * Sample Game Object Behaviors
 */
class OutlineBehavior : public Component {
    public:
    void init() override {
        Renderer* r = getGameObject()->getComponent<Renderer>();
        r->getOutlineConfig().color_ = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    void update() override {
        KeyboardHandler* k = getGameObject()->getGame()->getKeyboardHandler();
        if (k->getKeyDown(GLFW_KEY_O)) {
            outlined = !outlined;
            Renderer* r = getGameObject()->getComponent<Renderer>();
            if (outlined) {
                r->getOutlineConfig().active_ = true;
            } else {
                r->getOutlineConfig().active_ = false;
            }
        }
    }
    bool outlined = false;
};

class FlashLight : public Component {
    public:
    void init() override {
        SpotLight* sl = getGameObject()->getComponent<SpotLight>();
        sl->flipOnOff();
    }
    void update() override {
        KeyboardHandler* k = getGameObject()->getGame()->getKeyboardHandler();
        SpotLight* sl = getGameObject()->getComponent<SpotLight>();
        if (k->getKeyDown(GLFW_KEY_F)) {
            sl->flipOnOff();
        }
        Transform* t = getGameObject()->getComponent<Transform>();
        sl->setDirection(t->getWorldRotation() * glm::vec3(1.0f, 0.0f, 0.0f));
    }
    bool on_ = false;
};

int main() {
    /*
     * Game Intialization
     */
    Game game(1920, 1080);

    /*
     * Load needed Assets
     */

    // Shaders
    ShaderID flatShader = AssetManager::loadShader(
            PathUtils::shaderDir / "flat/flat.vert",
            PathUtils::shaderDir / "flat/flat.frag");
    ShaderID phongShader = AssetManager::loadShader(
            PathUtils::shaderDir / "phong/phong.vert",
            PathUtils::shaderDir / "phong/phong.frag");
    ShaderID blurEffectShader = AssetManager::loadShader(
            PathUtils::shaderDir / "postprocess/screen.vert",
            PathUtils::shaderDir / "postprocess/blurScreen.frag");
    ShaderID reflectShader = AssetManager::loadShader(
            PathUtils::shaderDir / "reflect/reflect.vert",
            PathUtils::shaderDir / "reflect/reflect.frag");
    ShaderID explodeShader = AssetManager::loadShader(
            PathUtils::shaderDir / "explode/explode.vert",
            PathUtils::shaderDir / "explode/explode.frag",
            PathUtils::shaderDir / "explode/explode.geom");
    //game.setPostProcessingEffect(blurEffectShader);
    
    // Textures
    TextureID containerDiffuse = AssetManager::loadTexture(
            PathUtils::textureDir / "container2.jpg");
    TextureID containerSpecular = AssetManager::loadTexture(
            PathUtils::textureDir / "container2_specular.jpg");
    TextureID grassDiffuse = AssetManager::loadTexture(
            PathUtils::textureDir / "grass15.png");

    std::filesystem::path cubeDir = PathUtils::textureDir / "sky";
    TextureID skyCubemap = AssetManager::loadCubemap(
            {cubeDir / "right.jpg", cubeDir / "left.jpg", 
            cubeDir / "top.jpg", cubeDir / "bottom.jpg",
            cubeDir / "front.jpg", cubeDir / "back.jpg"});
    game.setSkybox(skyCubemap);
    
    // Materials
    Material containerMaterial;
    containerMaterial.diffuse_ = containerDiffuse;
    containerMaterial.specular_ = containerSpecular;
    containerMaterial.shininess_ = 32.0f;

    float floorSize = 10000.0f;
    float tileSize = 4.0f;
    Material grassMaterial;
    grassMaterial.diffuse_ = grassDiffuse;
    grassMaterial.shininess_ = 32.0f;
    grassMaterial.tesselationRate_ = floorSize / tileSize;

    MaterialID containerMatID = AssetManager::storeMaterial(containerMaterial);
    MaterialID grassMatID = AssetManager::storeMaterial(grassMaterial);

    // Meshes
    MeshID floorMesh = AssetManager::storeMesh(Shapes::createPlane(1, 1, floorSize, floorSize));

    // Models
    ModelID backpackModel = AssetManager::loadModel(PathUtils::objectDir / "backpack/backpack.obj");
    ModelID containerModel = AssetManager::storeModel({{
            {AssetManager::defaultMeshes().cube_, containerMatID}
            }});
    ModelID cubeModel = AssetManager::storeModel({{
            {AssetManager::defaultMeshes().cube_, AssetManager::defaultMaterials().textureless_}
            }});
    ModelID floorModel = AssetManager::storeModel({{
            {floorMesh, grassMatID}
            }});
    
    // Game Objects
    GameObject lightCube(&game);
    GameObject backpack(&game);
    GameObject container(&game);
    GameObject mirrorContainer(&game);
    GameObject floor(&game);
    GameObject flashlight(&game);
    GameObject sunlight(&game);

    Transform* t;
    Renderer* r;
   
    /*
     * Describe Scene
     */

    // light cube
    t = lightCube.addComponent<Transform>();
    t->translate(glm::vec3(10.0f, -1.0f, 0.0f));
    r = lightCube.addComponent<Renderer>(cubeModel, AssetManager::defaultShaders().fallback_);
    AreaLight* l = lightCube.addComponent<AreaLight>();
    game.addGameObject(&lightCube);
    game.addLightSource(l);

    // backpack
    t = backpack.addComponent<Transform>();
    r = backpack.addComponent<Renderer>(backpackModel, phongShader);
    //r->getDebugConfig().visualizeNormals_ = true;
    t->translate(glm::vec3(10.0f, -1.0f, -5.0f));
    game.addGameObject(&backpack);

    // container
    t = container.addComponent<Transform>();
    r = container.addComponent<Renderer>(containerModel, phongShader);
    t->translate(glm::vec3(10.0f, -1.0f, 5.0f));
    t->scale(glm::vec3(4.0f, 4.0f, 4.0f));
    container.addComponent<OutlineBehavior>();
    game.addGameObject(&container);

    // floor
    t = floor.addComponent<Transform>();
    r = floor.addComponent<Renderer>(floorModel, phongShader);
    t->translate(glm::vec3(01.0f, -3.0f, 0.0f));
    game.addGameObject(&floor);

    // flashlight
    t = flashlight.addComponent<Transform>();
    SpotLight* s = flashlight.addComponent<SpotLight>();
    flashlight.addComponent<FlashLight>();
    game.getCamera().addChild(&flashlight);
    t->translate(glm::vec3(1.0f,-0.2f, 0.5f));
    game.addGameObject(&flashlight);
    game.addLightSource(s);

    // mirror
    t = mirrorContainer.addComponent<Transform>();
    t->translate(glm::vec3(1.0f, -1.0f, 7.0f));
    t->scale(glm::vec3(4,4,4));
    r = mirrorContainer.addComponent<Renderer>(containerModel, reflectShader);
    game.addGameObject(&mirrorContainer);

    DirectionalLight* d = sunlight.addComponent<DirectionalLight>();
    game.addLightSource(d);
    game.addGameObject(&sunlight);

    game.Loop();
    
}
