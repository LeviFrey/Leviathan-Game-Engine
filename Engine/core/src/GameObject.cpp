#include "GameObject.h"
#include <memory>


void GameObject::update() {
    for (auto& [type, component] : component_storage_->getData()) {
        component->update();
    }

    for (auto& child : children_) {
        child->update();
    }
}
