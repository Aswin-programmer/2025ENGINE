#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif


#include <iostream>

#include "../../WINDOW/Window.h"
#include "../../RENDERER/SHADERS/Shader.h"

class ENGINE_API DepthMap
{
public:
	DepthMap() = default;
	DepthMap(int depthMapWidth, int depthMapHeight);
	GLuint GetTextureID() { return depthTexture; }
	void BindDepthTextureToTextureUnit(int unit);
	void DepthMapInit();
	void DepthMapBind();
	void DepthMapUnBind();
	void RenderDebugDepthMap(Shader& depthDebugShader);

private:
	GLuint fbo = 0;
	GLuint depthTexture = 0;
	GLuint fullScreenVAO = 0;
	int depthMapWidth;
	int depthMapHeight;
};
