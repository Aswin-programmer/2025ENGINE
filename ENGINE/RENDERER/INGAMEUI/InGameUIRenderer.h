#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <OliUI.h>

#include "../SHADERS/Shader.h"
#include "../TEXTURE_KTX/TextureLoader.h"
#include "../TEXTURE_KTX/TextureKTX2.h"

constexpr uint32_t INGAMEUI_MAX_TRIANGES = 10000;
constexpr uint32_t INGAMEUI_MAX_VERTICES = INGAMEUI_MAX_TRIANGES * 3;


struct InGameUIVertex 
{
public:
    InGameUIVertex(glm::vec3 position_, glm::vec2 texCoord_) 
        : 
        position{position_}, 
        texCoord{texCoord_}
    {

    }

private:
    glm::vec3 position;
    glm::vec2 texCoord;
};

struct InGameUISpriteRect
{
    InGameUISpriteRect(){}
    InGameUISpriteRect(int x_, int y_, int w_, int h_)
        :
        x{x_},
        y{y_},
        w{w_},
        h{h_}
    {
        
    }
    int x;
    int y;
    int w;
    int h;
};

struct InGameUISprite
{
public:
    InGameUISprite(){}
    InGameUISprite(
        std::string spriteName_, 
        int width_, int height_, 
        std::unordered_map<std::string, InGameUISpriteRect> sprites_
        )
        :
        spriteName{spriteName_},
        width{width_}, height{height_},
        sprites{sprites_}
    {
        
    }
    std::string spriteName;
    int width;
    int height;
    std::unordered_map<std::string, InGameUISpriteRect> sprites;
};

class ENGINE_API InGameUIRenderer
{
public:
    InGameUIRenderer();
    InGameUIRenderer(
        std::string spriteName, 
        InGameUISprite inGameUISprite,
        std::string vertexShaderPath,
        std::string fragmentShaderPath
    );
    ~InGameUIRenderer();

    void InitializeInGameUIRenderer();
    void OliUIInspectDrawCommands(OliUIMainContext* oliUIMainContext);
    void CleanUpInGameUIRenderer();
    void RenderInGameUI();

private:
    GLuint inGameUIVAO;
    GLuint inGameUIVBO;

    std::vector<InGameUIVertex> inGameUIVertices;
    InGameUISprite inGameUISprite;
    std::shared_ptr<TextureKTX2> inGameUISpriteTexture;
    std::shared_ptr<Shader> inGameUIShader;
};