#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Image::Image(const unsigned char* data, int width, int height, int num_channels) :
    data_(std::make_unique<unsigned char[]>(width * height * num_channels)), 
    width_(width), 
    height_(height), 
    num_channels_(num_channels) 
{
    std::memcpy(data_.get(), data, width_ * height_ * num_channels_);
}

Image::Image(int width, int height, int num_channels) :
    data_(nullptr),
    width_(width),
    height_(height),
    num_channels_(num_channels)
{}


Image::Image(const std::filesystem::path& path) : path_(path) {
    unsigned char* data = stbi_load(path_.c_str(), &width_, &height_, &num_channels_, 0);

    if (!data) {
        throw std::runtime_error("Failed to load image: " + path.string());
    }
            
    data_ = std::make_unique<unsigned char[]>(width_ * height_ * num_channels_);
    std::memcpy(data_.get(), data, width_ * height_ * num_channels_);
    stbi_image_free(data);
}

