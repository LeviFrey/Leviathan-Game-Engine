#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Components/Component.h"

class GameObject {
    public:
        GameObject() = default;
        
        // Ran in Game Loop, ran each frame.
        void updateComponents();
        
        void setParent(GameObject* other);

        //Component::ComponentSet* getComponents();

        template<typename T, typename... Args>
        T* addComponent(Args&&... args) {
            T* component = component_storage_->insert<T>(std::forward<Args>(args)...);
            component->setComponentNode(component_graph_node_.get());
            return component;
        }

        const Component::ComponentSet* getComponents() const {
            return component_storage_.get();
        }


    private:

        std::unique_ptr<Component::ComponentSet> component_storage_ = 
            std::make_unique<Component::ComponentSet>();

        std::unique_ptr<Component::ComponentNode> component_graph_node_ = 
            std::make_unique<Component::ComponentNode>(component_storage_.get());
};
