#pragma once

#include <string>

#include <glyph.h>

class TextRenderer
{
public:
	TextRenderer(std::string filePath, int windowWidth, int windowHeight);
	~TextRenderer();

	void InitTextRenderer();
	void TextRendererDraw(std::string text, float posX, float posY, float scale
		, float r, float g, float b, int effects);
private:
	glyph_renderer_t textRenderer;
	std::string filePath;
	int windowWidth;
	int windowHeight;

};