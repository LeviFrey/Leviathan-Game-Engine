#include "Engine.h"

int main() {
    int window_width = 1000;
    int window_height = 1000;
    
    Game game(window_width, window_height);
    
    Shader flat_shader(
        PathUtils::shaderDir / "flat/flat.vert",
        PathUtils::shaderDir / "flat/flat.frag"
    );

    Texture diffuse(PathUtils::textureDir / "container2.jpg");
    Texture specular(PathUtils::textureDir / "container2_specular.jpg");
    PhongMaterial phong_mat(&diffuse, &specular);

    Model basic_model;
    Mesh cube = Shapes::createCube(1.0f);
    
    basic_model.addPart(&cube, &phong_mat);
    
    // Create and add behavior for container
    GameObject test_obj;
    test_obj.addComponent<Renderer>(&basic_model, &flat_shader);
    Transform* t = test_obj.addComponent<Transform>();
    t->translate(glm::vec3(5.0f, 0.0f, 0.0f));

    game.addGameObject(&test_obj);

    game.Loop();
    return 0;
}
