#include "Engine.h"

int main() {
    int window_width = 1000;
    int window_height = 1000;
    Game game(window_width, window_height);

    Texture diffuse = TextureLoader::loadTextureFromFile(
        PathUtils::textureDir / "container2.jpg", {});
    Texture specular = TextureLoader::loadTextureFromFile(
        PathUtils::textureDir / "container2_specular.jpg", {});

    Material material;
    material.diffuse_ = diffuse;
    material.specular_ = specular;
    material.shininess_ = 32.0f;

    Mesh mesh = Shapes::createCube(1.0f);
    
    Model model;
    model.addPart(mesh, material);
    
    Shader shader = ShaderLoader::loadShaderFromFile(
        PathUtils::shaderDir / "flat/flat.vert",
        PathUtils::shaderDir / "flat/flat.frag");

    GameObject container(&game);
    container.addComponent<Renderer>(&model, &shader);
    Transform* t = container.addComponent<Transform>();
    game.addGameObject(&container);

    GameObject camera(&game);
    t = camera.addComponent<Transform>();
    FreeCamera* fc = camera.addComponent<FreeCamera>();
    game.setCamera(fc);
    game.addGameObject(&camera);

    game.Loop();
    
    return 0;
}


