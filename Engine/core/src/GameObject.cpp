#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Game.h"
#include <memory>

Game* GameObject::getGame() const {
    return game_;
}

GameObject* GameObject::getParent() const {
    return parent_;
}

const std::unordered_set<GameObject*>& GameObject::getChildren() const {
    return children_;
}

void GameObject::updateComponents() {
    for (auto& [type, component] : components_) {
        component->update();
    }
}

void GameObject::addChild(GameObject* obj) {
    if (!obj || obj == this) { return; }
    if(obj->getParent()) {
        obj->getParent()->children_.erase(obj);
    }
    children_.insert(obj);
    obj->parent_ = this;
}
