#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <iostream>

template <typename T>
class TypeMap {
    public:
        
        // Access existing typed object
        template<typename S>
        S* get() const {
            static_assert(std::is_base_of_v<T, S>);
            auto it = data_.find(std::type_index(typeid(S)));
            if (it != data_.end()) {
                return static_cast<S*>(it->second.get());
            }
            std::cerr << "Type: " << typeid(S).name() << ", not found..." << std::endl;
            return nullptr;
        }
        
        // Check if set contains typed object
        template<typename S>
        bool contains() const {
            static_assert(std::is_base_of_v<T, S>);
            return data_.find(std::type_index(typeid(S))) != data_.end();
        }

        // Add a new typed object
        template<typename S, typename... Args>
        S* insert(Args&&... args) {
            static_assert(std::is_base_of_v<T, S>);
            auto comp = std::make_unique<S>(std::forward<Args>(args)...);
            S* ptr = comp.get();
            data_[std::type_index(typeid(S))] = std::move(comp);
            return ptr;
        }

        
        std::unordered_map<std::type_index, std::unique_ptr<T>>& getData() { return data_; }

    private:

        // Node data
        std::unordered_map<std::type_index, std::unique_ptr<T>> data_;

};
