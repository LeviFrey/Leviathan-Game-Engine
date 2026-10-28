#pragma once

#include "TypeMap.h"
#include "Hierarchy.h"


class Component {
    public:
        using ComponentSet = TypeMap<Component>;
        using ComponentNode = HierarchyNode<ComponentSet>;

        Component() = default;
        virtual ~Component() = default;
 
        virtual void update() {}
        
        // -- Component Hierarchy API --

        void setComponentNode(ComponentNode* node) { 
            comp_node_ = node; 
        }
        
        ComponentSet* getSiblingComponents() const {
            if (!comp_node_) {
                std::cerr << "No Node found for component" << std::endl;
                return nullptr;
            }
            return comp_node_->getData();
        }

        template <typename T>
        T* getSiblingComponent() const {
            return getSiblingComponents()->get<T>();
        }

        std::unordered_set<ComponentNode*> getChildrenComponentSets() {
            return comp_node_->getChildren();
        }
        
        bool hasParentComponents() const {
            return comp_node_->getParent() != nullptr;
        }

        ComponentSet* getParentComponents() const {
            return comp_node_->getParent()->getData();
        }

    private:
        ComponentNode* comp_node_ = nullptr;

};
