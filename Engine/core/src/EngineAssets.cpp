#include "EngineAssets.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "PathUtils.h"
#include "Shapes.h"
#include "AssetManager.h"

void EngineAssets::init() {

    // RENDER DATA CONFIG
    RenderDataConfig defaultRDC;
    defaultRDC.init();
    defaultRDC_ = AssetManager::storeAsset<RenderDataConfig>(defaultRDC);
    
    // SHADERS
    defaultShader_ = AssetManager::loadAssetID<Shader>({
            PathUtils::shaderDir / "default/default.vert",
            PathUtils::shaderDir / "default/default.frag"});

    noPostEffect_ = AssetManager::loadAssetID<Shader>({
            PathUtils::shaderDir / "postprocess/screen.vert",
            PathUtils::shaderDir / "postprocess/defaultScreen.vert"});

    outlineShader_ = AssetManager::loadAssetID<Shader>({
            PathUtils::shaderDir / "singleColor/singleColor.vert",
            PathUtils::shaderDir / "singleColor/singleColor.frag"});

    visualizeNormals_ = AssetManager::loadAssetID<Shader>({
            PathUtils::shaderDir / "normal/normal.vert",
            PathUtils::shaderDir / "normal/normal.frag"});

    
    // MESHES
    Mesh unitCube = Shapes::createCube(1.0f);
    unitCube_ = AssetManager::storeAsset<Mesh>(unitCube);
    
    // TEXTURES
    unsigned char black[] = {0, 0, 0, 0};
    Texture fallbackTexture = TextureLoader::loadTextureFromData(black, 1, 1, {});
    fallbackTexture_ = AssetManager::storeAsset<Texture>(fallbackTexture);
    
    // MATERIALS
    Material fallbackMaterial;
    fallbackMaterial.diffuse_ = fallbackTexture;
    fallbackMaterial.specular_ = fallbackTexture;
    fallbackMaterial_ = AssetManager::storeAsset<Material>(fallbackMaterial);
    
    // MODELS
    Model fallbackCube;
    fallbackCube.addPart(unitCube, fallbackMaterial);
    fallbackCube_ = AssetManager::storeAsset<Model>(fallbackCube); 
} 
