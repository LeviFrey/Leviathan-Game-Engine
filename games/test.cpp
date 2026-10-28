#include "Engine.h"

int main() {

    // -- Game Init --
    int window_width = 1000;
    int window_height = 1000;
    
    Game game(window_width, window_height);
    
    // -- Shader --
    Shader flat_shader(
        PathUtils::shaderDir / "flat/flat.vert",
        PathUtils::shaderDir / "flat/flat.frag"
    );
    
    // -- Texture --
    Texture diffuse(PathUtils::textureDir / "container2.jpg");
    Texture specular(PathUtils::textureDir / "container2_specular.jpg");
    PhongMaterial phong_mat(&diffuse, &specular);

    // -- Mesh --
    Mesh cube = Shapes::createCube(1.0f);

    // -- Model --
    Model basic_model;
    basic_model.addPart(&cube, &phong_mat);

    Model backpack(PathUtils::objectDir / "backpack/backpack.obj");
    
    // -- Game Object --
    GameObject test_obj;
    test_obj.addComponent<Renderer>(&backpack, &flat_shader);
    Transform* t = test_obj.addComponent<Transform>();
    t->translate(glm::vec3(5.0f, 0.0f, 0.0f));

    game.addGameObject(&test_obj);
    
    // -- Start Game --
    game.Loop();
    return 0;
}
