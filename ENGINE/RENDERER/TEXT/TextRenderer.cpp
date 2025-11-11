#include "TextRenderer.h"

TextRenderer::TextRenderer(std::string filePath, int windowWidth, int windowHeight)
	:
	filePath{filePath},
	windowHeight{windowHeight},
	windowWidth{windowWidth},
	textRenderer{NULL}
{

}

TextRenderer::~TextRenderer()
{
	glyph_renderer_free(&textRenderer);
}

void TextRenderer::InitTextRenderer()
{
	textRenderer = glyph_renderer_create(filePath.c_str(), 64.f, NULL, GLYPH_UTF8, NULL);
	glyph_renderer_set_projection(&textRenderer, windowWidth, windowHeight);
}

void TextRenderer::TextRendererDraw(std::string text, float posX, float posY, float scale
	, float r, float g, float b, int effects)
{
	glyph_renderer_draw_text(&textRenderer, text.c_str(), posX, posY, scale, r, g, b, effects);
}
