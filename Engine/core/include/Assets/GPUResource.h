#pragma once
#include <utility>

class GPUResource {
    public:

        virtual ~GPUResource() = default;

        unsigned int getResource() {
            if (!loaded() || dirty_) {
                unload();
                gpu_resource_ = loadResource();
                dirty_ = false;
            }
            return gpu_resource_;
        }
        
        const unsigned int& getResourceID() {
            return gpu_resource_;
        }

        bool loaded() {
            return gpu_resource_ != 0;
        }

        void unload() {
            if (loaded()) {
                deleteResource();
                invalidate();
            }
        }

        void invalidate() {
            gpu_resource_ = 0;
        }

        void takeResource(GPUResource&& other) {
            unload();
            gpu_resource_ = std::exchange(other.gpu_resource_, 0);
        }

        void makeDirty() {
            dirty_ = true;
        }

    private:
        virtual unsigned int loadResource() = 0;
        virtual void deleteResource() {}

        bool dirty_ = true;
        unsigned int gpu_resource_ = 0;
};
