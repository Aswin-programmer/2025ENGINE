#include "InGameUIRenderer.h"

InGameUIRenderer::InGameUIRenderer() 
	: 
	inGameUIVAO{0}, 
	inGameUIVBO{0}
{
	inGameUIVertices.reserve(INGAMEUI_MAX_VERTICES);
}

InGameUIRenderer::InGameUIRenderer(
    std::string spriteName, 
    InGameUISprite inGameUISprite,
    std::string vertexShaderPath,
    std::string fragmentShaderPath
)
    :
    inGameUISprite{inGameUISprite},
    inGameUISpriteTexture{TextureLoader::GetKTX2Texture(spriteName)},
    inGameUIShader{std::make_shared<Shader>(
        vertexShaderPath.c_str(), fragmentShaderPath.c_str()
    )}
{

}

InGameUIRenderer::~InGameUIRenderer() 
{
    if (inGameUIVBO != 0)
    {
        glDeleteBuffers(1, &inGameUIVBO);
        inGameUIVBO = 0;
    }

    if (inGameUIVAO != 0)
    {
        glDeleteVertexArrays(1, &inGameUIVAO);
        inGameUIVAO = 0;
    }
}

void InGameUIRenderer::InitializeInGameUIRenderer() 
{
	glCreateVertexArrays(1, &inGameUIVAO);

	glCreateBuffers(1, &inGameUIVBO);

	glNamedBufferStorage(
		inGameUIVBO,
		sizeof(InGameUIVertex) * INGAMEUI_MAX_VERTICES,
		nullptr,
		GL_DYNAMIC_STORAGE_BIT
	);

	glVertexArrayVertexBuffer(
		inGameUIVAO,
		0,
		inGameUIVBO,
		0,
		sizeof(InGameUIVertex)
	);

	// Position (location == 0)
	glVertexArrayAttribFormat(
		inGameUIVAO,
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0
	);
	glVertexArrayAttribBinding(inGameUIVAO, 0, 0);
	glEnableVertexArrayAttrib(inGameUIVAO, 0);

	// TexCoord (location == 1)
	glVertexArrayAttribFormat(
		inGameUIVAO,
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3)
	);
	glVertexArrayAttribBinding(inGameUIVAO, 1, 0);
	glEnableVertexArrayAttrib(inGameUIVAO, 1);
}

void InGameUIRenderer::OliUIInspectDrawCommands(OliUIMainContext* oliUIMainContext)
{
    for (int i = 0; i < oliUIMainContext->oliUIDrawCommandIndex; i++)
    {
        OliUIDrawCommand* cmd = &oliUIMainContext->oliUIDrawCommands[i];

        if (cmd->oliUIDrawCommandType == OliUIDrawCommandTexturedRect)
        {
            auto it = inGameUISprite.sprites.find(cmd->texture.textureName);

            if (it != inGameUISprite.sprites.end())
            {
                const InGameUISpriteRect& rect = it->second;

                float atlasW = static_cast<float>(inGameUISprite.width);
                float atlasH = static_cast<float>(inGameUISprite.height);

                // ---- Pixel Position (NO NDC conversion) ----
                float x = cmd->size.x;
                float y = cmd->size.y;
                float w = cmd->size.w;
                float h = cmd->size.h;

                // ---- Quad positions (pixel space) ----
                glm::vec3 bl = { x,     y + h, 0.0f };
                glm::vec3 br = { x + w, y + h, 0.0f };
                glm::vec3 tr = { x + w, y,     0.0f };
                glm::vec3 tl = { x,     y,     0.0f };

                // ---- Compute UVs (OpenGL corrected V flip) ----
                float uMin = rect.x / atlasW;
                float uMax = (rect.x + rect.w) / atlasW;

                float vMin = rect.y / atlasH;
                float vMax = (rect.y + rect.h) / atlasH;

                glm::vec2 uvBL = { uMin, vMin };
                glm::vec2 uvBR = { uMax, vMin };
                glm::vec2 uvTR = { uMax, vMax };
                glm::vec2 uvTL = { uMin, vMax };

                // ---- Triangle 1 ----
                inGameUIVertices.emplace_back(bl, uvBL);
                inGameUIVertices.emplace_back(br, uvBR);
                inGameUIVertices.emplace_back(tr, uvTR);

                // ---- Triangle 2 ----
                inGameUIVertices.emplace_back(bl, uvBL);
                inGameUIVertices.emplace_back(tr, uvTR);
                inGameUIVertices.emplace_back(tl, uvTL);
            }
        }
    }
}

void InGameUIRenderer::CleanUpInGameUIRenderer()
{
    inGameUIVertices.clear();
}

void InGameUIRenderer::RenderInGameUI()
{
    inGameUIShader->use();
    glm::mat4 projection = glm::ortho(
        0.0f, 640.f,   
        480.f, 0.0f,  
        -1.0f, 1.0f
    );
    inGameUIShader->setMat4("uProjection", projection);
    glBindVertexArray(inGameUIVAO);

    inGameUISpriteTexture->Bind(0);

    if (!inGameUIVertices.empty())
    {
        glNamedBufferSubData(
            inGameUIVBO,
            0,
            inGameUIVertices.size() * sizeof(InGameUIVertex),
            inGameUIVertices.data()
        );

        glBindVertexArray(inGameUIVAO);
        glDrawArrays(GL_TRIANGLES, 0,
            static_cast<GLsizei>(inGameUIVertices.size()));
    }
    inGameUIShader->unuse();
    glBindVertexArray(0);
}
