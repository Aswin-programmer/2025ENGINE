#include "DepthMap.h"

DepthMap::DepthMap(int depthMapWidth, int depthMapHeight)
	:
	depthMapWidth{depthMapWidth},
	depthMapHeight{depthMapHeight}
{
}

void DepthMap::BindDepthTextureToTextureUnit(int unit)
{
    glBindTextureUnit(unit, depthTexture);
}

void DepthMap::DepthMapInit()
{
    // Create depth texture (DSA)
    glCreateTextures(GL_TEXTURE_2D, 1, &depthTexture);
    glTextureStorage2D(depthTexture, 1, GL_DEPTH_COMPONENT32F, depthMapWidth, depthMapHeight);

    glTextureParameteri(depthTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(depthTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTextureParameterfv(depthTexture, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Create FBO
    glCreateFramebuffers(1, &fbo);

    // Attach depth texture
    glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    // No color render
    glNamedFramebufferDrawBuffer(fbo, GL_NONE);
    glNamedFramebufferReadBuffer(fbo, GL_NONE);

    // Check status
    if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("DepthMap FBO is incomplete!");
    }

    glCreateVertexArrays(1, &fullScreenVAO);
}

void DepthMap::DepthMapBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, depthMapWidth, depthMapHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void DepthMap::DepthMapUnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Window::getWidth(), Window::getHeight());
}

void DepthMap::RenderDebugDepthMap(Shader& depthDebugShader)
{
    depthDebugShader.use();
    glBindVertexArray(fullScreenVAO);
    BindDepthTextureToTextureUnit(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
