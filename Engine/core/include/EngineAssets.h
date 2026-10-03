#pragma once
#include "AssetTypes.h"


/*
 * TODO: consider adding a handler for assets that store whether it's been loaded or not.
 * Currently not really a problem, but later could be useful. would also decouple function from field
 * entry.
 */
class EngineAssets {
    public:
        static void init();
        
        // FALLBACK
        static const TextureID& getFallbackTexture() { return fallbackTexture_; }
        static const MaterialID& getFallbackMaterial() { return fallbackMaterial_; }

        // RENDER OPTIONS
        static const RenderDataConfigID& getDefaultRDC() { return defaultRDC_; }
        static const ShaderID& getDefaultShader() { return defaultShader_; }
        static const ShaderID& getNoPostEffect() { return noPostEffect_; }
        static const ShaderID& getOutlineShader() { return outlineShader_; }
        static const ShaderID& getVisualizeNormalShader() { return visualizeNormals_; }

        // UTILITY
        static const MeshID& getUnitCube() { return unitCube_; }
        static const ModelID& getFallBackCube() { return fallbackCube_; }
    private:

        // FALLBACK
        static TextureID fallbackTexture_;
        static MaterialID fallbackMaterial_;

        // RENDER OPTIONS
        static RenderDataConfigID defaultRDC_;
        static ShaderID defaultShader_;
        static ShaderID noPostEffect_;
        static ShaderID outlineShader_;
        static ShaderID visualizeNormals_;

        // Utility
        static MeshID unitCube_;
        static ModelID fallbackCube_;

};

