#pragma once

// standard
#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <memory>

// from Engine
#include "Assets/Mesh.h"
#include "Assets/Texture.h"
#include "Assets/Shader.h"
#include "Assets/Model.h"
#include "Assets/Material.h"
#include "AssetTypes.h"

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
            std::vector<std::unique_ptr<T>> data;
            std::unordered_map<std::string, AssetID<T>> registry;
        };
        
        // Data storages
        AssetStorage<Mesh> mesh_storage_;
        AssetStorage<Texture> texture_storage_;
        AssetStorage<Cubemap> cubemap_storage_;
        AssetStorage<Material> material_storage_;
        AssetStorage<Shader> shader_storage_;
        AssetStorage<Model> model_storage_;
        AssetStorage<VertexArrayObject> rdc_storage_;

        template <typename T>
        AssetStorage<T>& getStorage();

        // ---------------------------------------------------------------
        // Storage definitions for assets loaded through loader programs |
        // ---------------------------------------------------------------
        template <typename T>
        using AssetCache = std::map<AssetKey<T>, AssetID<T>>;
        
        // Data Caches
        AssetCache<Texture> texture_cache_;
        AssetCache<Cubemap> cubemap_cache_;
        AssetCache<Model> model_cache_;
        AssetCache<Shader> shader_cache_;

        template <typename T>
        AssetCache<T>& getCache();

        template <typename T>
        T loadOnCacheMiss(const AssetKey<T>& key);

    public:
        
        //  -- STORE: copy/move custom asset into AssetManager
        //  allows for:
        //  storeAsset<AssetType>(asset);
        //  storeAsset<AssetType>(std::move(Asset));
        //  storeAsset<AssetType>(AssetType(param1, param2, ...);
        template <typename T>
        AssetID<T> storeAsset(T asset, const std::string& nametag = {}) {
            auto& asset_storage = getStorage<T>();

            asset_storage.data.push_back(
                std::make_unique<T>(std::move(asset))
            );

            AssetID<T> asset_id{asset_storage.data.size() - 1};

            if (!nametag.empty()) {
                asset_storage.registry.try_emplace(nametag, asset_id);
            }

            return asset_id;
        }

        // -- EMPLACE: create asset within AssetManager with nametag
        template<typename T, typename... Args>
        AssetID<T> emplaceNamedAsset(const std::string nametag, Args&&... args) {
            AssetID<T> asset_id = emplaceAsset<T>(std::forward<Args>(args)...);
            
            auto& asset_storage = getStorage<T>();
            asset_storage.registry.try_emplace(nametag, asset_id);

            return asset_id;
        }
        
        // -- EMPLACE: create asset within AssetManager, no nametag
        template<typename T, typename... Args>
        AssetID<T> emplaceAsset(Args&&... args) {
            auto& asset_storage = getStorage<T>();

            asset_storage.data.emplace_back(
                std::make_unique<T>(std::forward<Args>(args)...)
            );

            AssetID<T> asset_id{asset_storage.data.size() - 1};

            return asset_id;
        }
        
        // GET custom asset by NAMETAG
        template <typename T>
        T& getAssetByName(const std::string& nametag) {
            auto& asset_storage = getStorage<T>();
            return *asset_storage.data.at(asset_storage.registry.at(nametag));
        }

        // GET custom asset by ID
        template <typename T>
        T& getAssetByID(const AssetID<T>& id) {
            auto& asset_storage = getStorage<T>();
            return *asset_storage.data.at(id.value);
        }

        // ACCESS asset ID from data cache for filesystem-loaded assets
        template <typename T>
        AssetID<T> loadAssetID(const AssetKey<T>& key) {
            auto& asset_cache = getCache<T>();

            auto it = asset_cache.find(key);
            if (it != asset_cache.end()) {
                return it->second;
            } else {
                T asset = loadOnCacheMiss<T>(key);  
                auto& asset_storage = getStorage<T>();
                asset_storage.data.push_back(std::make_unique<T>(std::move(asset)));

                AssetID<T> asset_id = {asset_storage.data.size() - 1};
                asset_cache.emplace(key, asset_id);

                return asset_id;
            }
        }

        // ACCESS asset from data cache for filesystem-loaded assets
        template <typename T>
        T& loadAsset(const AssetKey<T>& key) {
            AssetID<T> asset_id = loadAssetID<T>(key);
            return getAssetByID<T>(asset_id);
        }

};
