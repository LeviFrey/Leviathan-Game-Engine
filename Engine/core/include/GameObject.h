#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm/glm.hpp>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "Components/Component.h"
#include <unordered_set>

class Game;

class GameObject {
    public:

        GameObject(Game* game) { 
            game_ = game; 
        }
        GameObject() {}
        void updateComponents();

        Game* getGame() const;
        void setGame(Game* game) { game_ = game; };

        GameObject* getParent() const;
        const std::unordered_set<GameObject*>& getChildren() const;
        void addChild(GameObject* obj);

        virtual void init() {}

        template<typename T>
        T* getComponent() const {
            auto it = components_.find(std::type_index(typeid(T)));
            if (it != components_.end()) {
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        template<typename T>
        bool hasComponent() {
            return components_.find(std::type_index(typeid(T))) != components_.end();
        }

        template<typename T, typename... Args>
        T* addComponent(Args&&... args) { 
            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            comp->setGameObject(this);
            T* ptr = comp.get();
            ptr->init();
            components_[std::type_index(typeid(T))] = std::move(comp);
            return ptr;
        }

    private:
        Game* game_;
        std::unordered_map<std::type_index, std::unique_ptr<Component>> components_;
        GameObject* parent_ = nullptr;
        std::unordered_set<GameObject*> children_;
};

#endif
