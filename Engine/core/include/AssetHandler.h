#pragma once
template <typename T>
class Asset {
    public:
        Asset(T* asset) : asset_(asset) {}
        T* get() {
            if (!asset_) {
                return 0; // should return the default Engine value
            }
            return asset_;
        }
    private:
        T* asset_ = nullptr;
};
