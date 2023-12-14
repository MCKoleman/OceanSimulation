#pragma once
#include "rendering/texture.h"
#include "rendering/material.h"
#include "rendering/shader.h"
#include "rendering/iPrimitive.h"

struct renderTexture {
    std::string name;
    unsigned int ID;
    bool hasDepth;

    // constructor generates the shader on the fly
    renderTexture(const unsigned int _ID = -1, bool _hasDepth = false)
        : renderTexture((glCreateProgram()), _hasDepth) {}
    // constructor generates the shader on the fly
    renderTexture(const unsigned int _ID, bool _hasDepth = false)
    : ID(_ID), hasDepth(_hasDepth)
    {
        glGenFramebuffers(1, &ID);
        glBindFramebuffer(GL_FRAMEBUFFER, ID);

        unsigned int renderedTexture;
        glGenTextures(1, &renderedTexture);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (hasDepth) {
            unsigned int depthrenderbuffer;
            glGenRenderbuffers(1, &depthrenderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
        }
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Render Texture Process failed.");
        glBindFramebuffer(GL_FRAMEBUFFER, ID);
        glViewport(0, 0, 1024, 768);
    }
    // Cleans up the shader's info
    ~renderTexture()
    {
        glDeleteFramebuffers(1, &ID);
    }
    // activate the shader
    void Use()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1024, 768);

        if (hasDepth) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1024, 768, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        }
    }
};