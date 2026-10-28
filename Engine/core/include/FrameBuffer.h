#pragma once
#include "GPUResource.h"
#include "Texture.h"
#include <glad/glad.h>
#include <iostream>

class RenderBuffer : public GPUResource {
    public:

        RenderBuffer(unsigned int width, unsigned int height) :
            width_(width), height_(height)
        {}

    private:

        unsigned int loadResource() {
            unsigned int id;
            glGenRenderbuffers(1, &id);
            glBindRenderbuffer(GL_RENDERBUFFER, id);

            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH24_STENCIL8,
                width_, height_
            );

            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            return id;
        }

        unsigned int width_, height_;

};

class FrameBuffer : public GPUResource  {
    public:

        FrameBuffer(unsigned int width, unsigned int height) :
            width_(width), height_(height),
            render_buffer_(width_, height_),
            texture_(std::make_unique<Texture>(Image(width_, height_, 4)))
        {}

        void resize(unsigned int width, unsigned int height) {
            if (width == width_ && height == height_) { return; }

            render_buffer_ = RenderBuffer(width, height);
            
            *texture_ = Texture(Image(width, height, 4));

            if (loaded()) {
                bindMyTexture();
                bindMyRenderBuffer();
            }
        }

        Texture* getTexture() {
            return texture_.get();
        }

        void bind() {
            glBindFramebuffer(GL_FRAMEBUFFER, getResource());
        }

    private:

        void bindMyTexture() {
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                texture_->getResource(),
                0
            );
        }

        void bindMyRenderBuffer() {
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER,
                render_buffer_.getResource()
            );
        }
            

        unsigned int loadResource() {
            unsigned int id;
            glGenFramebuffers(1, &id);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            }
            
            bindMyTexture();
            bindMyRenderBuffer();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return id;
        }

        unsigned int width_, height_;
        RenderBuffer render_buffer_;
        std::unique_ptr<Texture> texture_;
};

