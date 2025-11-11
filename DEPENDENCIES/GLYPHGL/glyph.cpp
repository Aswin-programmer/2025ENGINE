#include "glyph.h"

glyph_renderer_t glyph_renderer_create(const char* font_path, float pixel_height, const char* charset, uint32_t char_type, glyph_effect_t* effect)
{
    glyph_effect_t default_effect = { (glyph_effect_type_t)GLYPH_NONE, NULL, NULL };
    if (effect == NULL) {
        effect = &default_effect;
    }
    glyph_renderer_t renderer = { 0 };

    if (!glyph_gl_load_functions()) {
#ifdef GLYPHGL_DEBUG
        GLYPH_LOG("Failed to load OpenGL functions\n");
#endif
        return renderer;
    }

    renderer.char_type = char_type;
    renderer.effect = *effect;
    renderer.atlas = glyph_atlas_create(font_path, pixel_height, charset, char_type);
    if (!renderer.atlas.chars || !renderer.atlas.image.data) {
#ifdef GLYPHGL_DEBUG
        GLYPH_LOG("Failed to create font atlas\n");
#endif
        return renderer;
    }

    unsigned char* red_channel = (unsigned char*)GLYPH_MALLOC(renderer.atlas.image.width * renderer.atlas.image.height);
    if (!red_channel) {
        glyph_atlas_free(&renderer.atlas);
        return renderer;
    }

    for (unsigned int i = 0; i < renderer.atlas.image.width * renderer.atlas.image.height; i++) {
        red_channel[i] = renderer.atlas.image.data[i * 3];
    }

    glGenTextures(1, &renderer.texture);
    glBindTexture(GL_TEXTURE_2D, renderer.texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, renderer.atlas.image.width, renderer.atlas.image.height,
        0, GL_RED, GL_UNSIGNED_BYTE, red_channel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLYPH_FREE(red_channel);

    if (renderer.effect.type == GLYPH_NONE) {
        renderer.shader = glyph__create_program(glyph__vertex_shader_source, glyph__fragment_shader_source);
    }
    else {
        renderer.shader = glyph__create_program(renderer.effect.vertex_shader, renderer.effect.fragment_shader);
    }
    if (!renderer.shader) {
        glDeleteTextures(1, &renderer.texture);
        glyph_atlas_free(&renderer.atlas);
        return renderer;
    }

    glyph__glGenVertexArrays(1, &renderer.vao);
    glyph__glGenBuffers(1, &renderer.vbo);
    glyph__glBindVertexArray(renderer.vao);
    glyph__glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glyph__glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 73728, NULL, GL_DYNAMIC_DRAW);
    glyph__glEnableVertexAttribArray(0);
    glyph__glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glyph__glEnableVertexAttribArray(1);
    glyph__glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glyph__glBindBuffer(GL_ARRAY_BUFFER, 0);
    glyph__glBindVertexArray(0);

    renderer.vertex_buffer_size = 73728 * 4; // Initial size for vertices (float * 4 per vertex)
    renderer.vertex_buffer = (float*)GLYPH_MALLOC(sizeof(float) * renderer.vertex_buffer_size);
    if (!renderer.vertex_buffer) {
        glyph__glDeleteVertexArrays(1, &renderer.vao);
        glyph__glDeleteBuffers(1, &renderer.vbo);
        glDeleteTextures(1, &renderer.texture);
        glyph__glDeleteProgram(renderer.shader);
        glyph_atlas_free(&renderer.atlas);
        return renderer;
    }

    renderer.initialized = 1;
    return renderer;
}

glyph_renderer_t glyph_renderer_create(const char* font_path, float pixel_height, const char* charset, uint32_t char_type, glyph_effect_t* effect);
void glyph_renderer_free(glyph_renderer_t* renderer)
{
    if (!renderer || !renderer->initialized) return;

    glyph__glDeleteVertexArrays(1, &renderer->vao);
    glyph__glDeleteBuffers(1, &renderer->vbo);
    glDeleteTextures(1, &renderer->texture);
    glyph__glDeleteProgram(renderer->shader);
    glyph_atlas_free(&renderer->atlas);
    GLYPH_FREE(renderer->vertex_buffer);

    renderer->initialized = 0;
}

void glyph_renderer_set_projection(glyph_renderer_t* renderer, int width, int height)
{
    if (!renderer || !renderer->initialized) return;

    float projection[16] = {
        2.0f / width, 0.0f, 0.0f, 0.0f,
        0.0f, -2.0f / height, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f
    };

    glyph__glUseProgram(renderer->shader);
    glyph__glUniformMatrix4fv(glyph__glGetUniformLocation(renderer->shader, "projection"), 1, GL_FALSE, projection);
    glyph__glUseProgram(0);
}

void glyph_renderer_update_projection(glyph_renderer_t* renderer, int width, int height)
{
    if (!renderer || !renderer->initialized) return;

    float projection[16] = {
        2.0f / width, 0.0f, 0.0f, 0.0f,
        0.0f, -2.0f / height, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f
    };

    glyph__glUseProgram(renderer->shader);
    glyph__glUniformMatrix4fv(glyph__glGetUniformLocation(renderer->shader, "projection"), 1, GL_FALSE, projection);
    glyph__glUseProgram(0);
}

void glyph_renderer_draw_text(glyph_renderer_t* renderer, const char* text, float x, float y, float scale,
    float r, float g, float b, int effects)
{
    if (!renderer || !renderer->initialized) return;

    glyph__glUseProgram(renderer->shader);
    glyph__glBindVertexArray(renderer->vao);
    glyph__glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->texture);
    glyph__glUniform1i(glyph__glGetUniformLocation(renderer->shader, "textTexture"), 0);
    glyph__glUniform3f(glyph__glGetUniformLocation(renderer->shader, "textColor"), r, g, b);
    glyph__glUniform1i(glyph__glGetUniformLocation(renderer->shader, "effects"), effects);

    size_t text_len = strlen(text);
    size_t required_size = sizeof(float) * 24 * text_len * 3; // Estimate based on max effects (normal + bold + underline)
    if (required_size > renderer->vertex_buffer_size) {
        size_t new_size = required_size * 2; // Double the size to minimize reallocations
        float* new_buffer = (float*)GLYPH_REALLOC(renderer->vertex_buffer, new_size);
        if (!new_buffer) return;
        renderer->vertex_buffer = new_buffer;
        renderer->vertex_buffer_size = new_size;
    }
    float* vertices = renderer->vertex_buffer;
    size_t vertex_count = 0;

    float current_x = x;
    size_t i = 0;
    while (i < text_len) {
        int codepoint;
        if (renderer->char_type == GLYPH_UTF8) {
            codepoint = glyph_utf8_decode(text, &i);
        }
        else {
            codepoint = (unsigned char)text[i];
            i++;
        }
        glyph_atlas_char_t* ch = glyph_atlas_find_char(&renderer->atlas, codepoint);
        if (!ch) {
            ch = glyph_atlas_find_char(&renderer->atlas, '?');
        }
        if (!ch || ch->width == 0) {
            current_x += ch ? ch->advance * scale : (renderer->atlas.pixel_height * 0.5f * scale);
            continue;
        }

        float xpos = current_x + ch->xoff * scale;
        float ypos = y - ch->yoff * scale;
        float w = ch->width * scale;
        float h = ch->height * scale;

        float tex_x1 = (float)ch->x / renderer->atlas.image.width;
        float tex_y1 = (float)ch->y / renderer->atlas.image.height;
        float tex_x2 = (float)(ch->x + ch->width) / renderer->atlas.image.width;
        float tex_y2 = (float)(ch->y + ch->height) / renderer->atlas.image.height;

        float glyph_vertices[24] = {
            xpos,     ypos + h,   tex_x1, tex_y2,
            xpos,     ypos,       tex_x1, tex_y1,
            xpos + w, ypos,       tex_x2, tex_y1,

            xpos,     ypos + h,   tex_x1, tex_y2,
            xpos + w, ypos,       tex_x2, tex_y1,
            xpos + w, ypos + h,   tex_x2, tex_y2
        };

        if (effects & GLYPHGL_ITALIC) {
            float shear = 0.2f;
            glyph_vertices[0] -= shear * h;
            glyph_vertices[12] -= shear * h;
            glyph_vertices[20] -= shear * h;
        }

        memcpy(vertices + vertex_count * 4, glyph_vertices, sizeof(glyph_vertices));
        vertex_count += 6;

        if (effects & GLYPHGL_BOLD) {
            float bold_offset = 1.0f * scale;
            float bold_vertices[24] = {
                xpos + bold_offset,     ypos + h,   tex_x1, tex_y2,
                xpos + bold_offset,     ypos,       tex_x1, tex_y1,
                xpos + w + bold_offset, ypos,       tex_x2, tex_y1,

                xpos + bold_offset,     ypos + h,   tex_x1, tex_y2,
                xpos + w + bold_offset, ypos,       tex_x2, tex_y1,
                xpos + w + bold_offset, ypos + h,   tex_x2, tex_y2
            };

            if (effects & GLYPHGL_ITALIC) {
                float shear = 0.2f;
                bold_vertices[0] -= shear * h;
                bold_vertices[12] -= shear * h;
                bold_vertices[20] -= shear * h;
            }

            memcpy(vertices + vertex_count * 4, bold_vertices, sizeof(bold_vertices));
            vertex_count += 6;
        }

        if (effects & GLYPHGL_UNDERLINE) {
            float underline_y = y + h * 0.1f;
            float underline_vertices[24] = {
                current_x, underline_y + 2, 0.0f, 0.0f,
                current_x, underline_y,     0.0f, 0.0f,
                current_x + ch->advance * scale, underline_y,     0.0f, 0.0f,

                current_x, underline_y + 2, 0.0f, 0.0f,
                current_x + ch->advance * scale, underline_y,     0.0f, 0.0f,
                current_x + ch->advance * scale, underline_y + 2, 0.0f, 0.0f
            };
            memcpy(vertices + vertex_count * 4, underline_vertices, sizeof(underline_vertices));
            vertex_count += 6;
        }

        current_x += ch->advance * scale;
    }

    glyph__glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glyph__glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_count * 4 * sizeof(float), vertices);
    glyph__glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, vertex_count);


    glyph__glBindVertexArray(0);
    glyph__glUseProgram(0);
}

GLuint glyph_renderer_get_vao(glyph_renderer_t* renderer) {
    return renderer->vao;
}

GLuint glyph_renderer_get_vbo(glyph_renderer_t* renderer) {
    return renderer->vbo;
}

GLuint glyph_renderer_get_shader(glyph_renderer_t* renderer) {
    return renderer->shader;
}

int glyph_utf8_decode(const char* str, size_t* index) {
    size_t i = *index;
    unsigned char c = (unsigned char)str[i++];
    if (c < 0x80) {
        *index = i;
        return c;
    }
    else if ((c & 0xE0) == 0xC0) {
        unsigned char c2 = (unsigned char)str[i++];
        *index = i;
        return ((c & 0x1F) << 6) | (c2 & 0x3F);
    }
    else if ((c & 0xF0) == 0xE0) {
        unsigned char c2 = (unsigned char)str[i++];
        unsigned char c3 = (unsigned char)str[i++];
        *index = i;
        return ((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
    }
    else if ((c & 0xF8) == 0xF0) {
        unsigned char c2 = (unsigned char)str[i++];
        unsigned char c3 = (unsigned char)str[i++];
        unsigned char c4 = (unsigned char)str[i++];
        *index = i;
        return ((c & 0x07) << 18) | ((c2 & 0x3F) << 12) | ((c3 & 0x3F) << 6) | (c4 & 0x3F);
    }
    *index = i;
    return 0xFFFD;
}

