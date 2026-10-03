#pragma once

// standard
#include <string>
#include <map>
#include <vector>
#include <filesystem>

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

        // ------------------------------------
        // Storage definitions for all assets |
        // ------------------------------------
        
        template <typename T>
        struct AssetStorage {
            std::vector<T> data_;
            std::unordered_map<std::string, AssetID<T>> registry_;
        };
        
        // Data storages
        inline static AssetStorage<Mesh> mesh_storage_;
        inline static AssetStorage<Texture> texture_storage_;
        inline static AssetStorage<Material> material_storage_;
        inline static AssetStorage<Shader> shader_storage_;
        inline static AssetStorage<Model> model_storage_;
        inline static AssetStorage<RenderDataConfig> rdc_storage_;

        template <typename T>
        static AssetStorage<T>& getStorage();

        // ---------------------------------------------------------------
        // Storage definitions for assets loaded through loader programs |
        // ---------------------------------------------------------------
        template <typename T>
        struct AssetCache {
            std::map<AssetKey<T>, AssetID<T>> cache_;
        };
        
        // Data Caches
        inline static AssetCache<Texture> texture_cache_;
        inline static AssetCache<Cubemap> cubemap_cache_;
        inline static AssetCache<Model> model_cache_;
        inline static AssetCache<Shader> shader_cache_;

        template <typename T>
        static AssetCache<T> getCache();

        template <typename T>
        static T loadOnCacheMiss(const AssetKey<T>& key);

    public:
        
        // STORE custom asset by NAMETAG
        template <typename T>
        static AssetID<T> storeAssetByName(T asset, const std::string& nametag) {
            auto& asset_storage = getStorage<T>();
            asset_storage.data_.push_back(asset);
            AssetID<T> id = {asset_storage.data_.size() - 1};
            asset_storage.registry_.insert({nametag, id});
            return id;
        }

        // STORE custom asset without specific nametag
        template <typename T>
        static AssetID<T> storeAsset(T asset) {
            auto& asset_storage = getStorage<T>();
            asset_storage.data_.push_back(asset);
            return {asset_storage.data_.size() - 1};
        }
        
        // GET custom asset by NAMETAG
        template <typename T>
        static T& getAssetByName(const std::string& nametag) {
            auto& asset_storage = getStorage<T>();
            return asset_storage.data_.at(asset_storage.registry_.at(nametag));
        }

        // GET custom asset by ID
        template <typename T>
        static T& getAssetByID(const AssetID<T>& id) {
            auto& asset_storage = getStorage<T>();
            return asset_storage.data_.at(id.value_);
        }

        
        // ACCESS asset from data cache for filesystem-loaded assets
        template <typename T>
        static T& loadAsset(const AssetKey<T>& key) {
            auto& asset_cache = getCache<T>();
            auto& asset_storage = getStorage<T>();
            auto it = asset_cache.find(key);
            if (it != asset_cache.end()) {
                return getAssetByID(it.second);
            } else {
                asset_storage.data_.push_back(loadOnCacheMiss<T>(key));
                AssetID<T> id = asset_storage.data_.size()-1;
                asset_cache.emplace(key, id);
                return getAssetByID(id);
            }
        }
        
        // ACCESS asset ID from data cache for filesystem-loaded assets
        template <typename T>
        static AssetID<T> loadAssetID(const AssetKey<T>& key) {
            auto& asset_cache = getCache<T>();
            auto it = asset_cache.find(key);
            if (it != asset_cache.end()) {
                return it->second;
            } else {
                auto& asset_storage = getStorage<T>();
                asset_storage.data_.push_back(loadOnCacheMiss<T>(key));
                AssetID<T> id = {asset_storage.data_.size() - 1};
                asset_cache.emplace(key, id);
                return id;
            }
        }

};
