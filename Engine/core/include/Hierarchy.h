#pragma once
#include <unordered_set>

template <typename T>
class HierarchyNode {
    public:
        HierarchyNode(T* data) : data_(data) {}

        void setParent(HierarchyNode* node) {

            // Remove from parent list
            if (node->getParent()) {
                if (hasDescendant(node)) { return; }
         
                node->getParent()->getChildren().erase(node);
            }

            parent_ = node;
        }

        HierarchyNode* getParent() { return parent_; }
        std::unordered_set<HierarchyNode*>& getChildren() { return children_; }
        T* getData() { return data_; }

    private:
        T* data_ = nullptr;
        HierarchyNode* parent_ = nullptr;
        std::unordered_set<HierarchyNode*> children_;

        bool hasDescendant(HierarchyNode* node) {
            for (
                HierarchyNode* curr_parent = node->getParent(); 
                curr_parent != nullptr; 
                curr_parent = curr_parent->getParent()
                ) 
            { 
                if (curr_parent == this) { return true; } 
            }

            return false;
        }
};
