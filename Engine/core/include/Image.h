#pragma once

#include <memory>
#include <cstring>
#include <filesystem>


class Image {
    public:

        // create image from data
        Image(const unsigned char* data, int width, int height, int num_channels);
        
        // create empty image (mostly used for buffers)
        Image(int width, int height, int num_channels);
        
        // create image from filesystem data
        Image(const std::filesystem::path& path);

        Image(Image&& other) noexcept = default;
        Image(const Image& other) = delete;
        Image& operator=(const Image& other) = delete;

        const unsigned char* getData() const { return data_.get(); }

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }
        const std::filesystem::path& getPath() { return path_; }
        int getNumChannels() const { return num_channels_; }

    private:
        std::filesystem::path path_;
        std::unique_ptr<unsigned char[]> data_;
        int width_, height_, num_channels_;
};


