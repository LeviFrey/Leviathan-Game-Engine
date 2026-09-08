#pragma once

// standard
#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <iostream>
#include <typeinfo>

// from Engine
#include "Mesh.h"
#include "Texture.h"
#include "Cubemap.h"
#include "Shader.h"
#include "Model.h"
#include "AssetTypes.h"
#include "Material.h"
#include "Components/Renderer.h"

// Used for Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

template <typename T>
        struct AssetKey;

        template<>
        struct AssetKey<Shader> {
            std::filesystem::path vertex;
            std::filesystem::path fragment;
            std::filesystem::path geometry;
            auto operator<(const AssetKey<Shader>& other) const  {
                return
                    std::tie(
                        vertex,
                        fragment,
                        geometry)
                     < std::tie(
                        other.vertex,
                        other.fragment,
                        other.geometry);
            }
        };
        
        template<>
        struct AssetKey<Cubemap> {
            std::array<std::filesystem::path, 6> paths;
            bool operator<(const AssetKey<Cubemap>& other) const {
                return paths < other.paths;
            }
        };

        template<>
        struct AssetKey<Texture> {
            std::filesystem::path path;
            bool operator<(const AssetKey<Texture>& other) const {
                return path < other.path;
            }
        };

        template<>
        struct AssetKey<Model> {
            std::filesystem::path path;
            bool operator<(const AssetKey<Model>& other) const {
                return path < other.path;
            }
        };


/*
 * A global storage for all game assets
 * If an Asset is needed between objects, AssetManager will cache them and can be loaded separately
 */
class AssetManager {
    private: 

        // Storage definitions for custom generated assets
        template <typename T>
        struct AssetStorage {
            std::vector<T> data_;
            std::unordered_map<std::string, int> registry_;
        };
    
        inline static AssetStorage<Mesh> mesh_storage_;
        inline static AssetStorage<Texture> texture_storage_;
        inline static AssetStorage<Material> material_storage_;
        inline static AssetStorage<Shader> shader_storage_;
        inline static AssetStorage<Model> model_storage_;
        inline static AssetStorage<RenderDataConfig> rdc_storage_;

        template <typename T>
        static AssetStorage<T>& getStorage();

        // Storage definitions for assets loaded through loader programs
        
        // Data Caches
        inline static std::map<AssetKey<Texture>, Texture> texture_cache_;
        inline static std::map<AssetKey<Cubemap>, Cubemap> cubemap_cache_;
        inline static std::map<AssetKey<Model>,   Model> model_cache_;
        inline static std::map<AssetKey<Shader>,  Shader> shader_cache_;

        template <typename T>
        static std::map<AssetKey<T>, T> getCache();

        template <typename T>
        static T loadOnCacheMiss(const AssetKey<T>& key);

    public:
        
        // STORE custom asset by NAMETAG
        template <typename T>
        static void storeAsset(T asset, const std::string& nametag) {
            auto& asset_storage = getStorage<T>();
            asset_storage.data_.push_back(asset);
            asset_storage.registry_.insert({nametag, asset_storage.size()-1});
        }
        
        // GET custom asset by NAMETAG
        template <typename T>
        static T& getAsset(const std::string& nametag) {
            auto& asset_storage = getStorage<T>();
            return asset_storage.data_.at(asset_storage.registry_.at(nametag));
        }
        
        // ACCESS data cache for program-loaded assets
        template <typename T>
        static T& accessAssetCache(const AssetKey<T>& key) {
            auto& asset_cache = getCache<T>();
            auto it = asset_cache.find(key);
            if (it != asset_cache.end()) {
                return it->second;
            } else {
                auto& asset_cache = getCache<T>();
                auto [it, inserted] = asset_cache.emplace(key, loadOnCacheMiss<T>(key));
                return it->second;
            }
        }
};
