#include "GameObject.h"
#include <memory>

void GameObject::setParent(GameObject* other) {
    component_graph_node_->setParent(other->component_graph_node_.get());
}

void GameObject::updateComponents() {
    for (auto& [type, component] : component_storage_->getData()) {
        component->update();
    }
}
