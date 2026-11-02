#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Components/Component.h"

class GameObject {
    public:
        GameObject() = default;
        
        // Ran in Game Loop, ran each frame.
        void update();

        template <typename T, typename... Args>
        T* addChild(Args&&... args) {
            std::unique_ptr<T> child = std::make_unique<T>(std::forward<Args>(args)...);
            child->component_graph_node_->setParent(component_graph_node_.get());
            T* child_ptr = child.get();
            children_.push_back(std::move(child));
            return child_ptr;
        }
        
        GameObject* addEmptyChild() {
            return addChild<GameObject>();
        }

        template <typename T, typename... Args>
        T* addComponent(Args&&... args) {
            T* component = component_storage_->insert<T>(std::forward<Args>(args)...);
            component->setComponentNode(component_graph_node_.get());
            return component;
        }

        template <typename T>
        T* getComponent() {
            return component_storage_->get<T>();
        }

        const Component::ComponentSet* getComponents() const {
            return component_storage_.get();
        }


    private:

        std::vector<std::unique_ptr<GameObject>> children_;

        std::unique_ptr<Component::ComponentSet> component_storage_ = 
            std::make_unique<Component::ComponentSet>();

        std::unique_ptr<Component::ComponentNode> component_graph_node_ = 
            std::make_unique<Component::ComponentNode>(component_storage_.get());
};
