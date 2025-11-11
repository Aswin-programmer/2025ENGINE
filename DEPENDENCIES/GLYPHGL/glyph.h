/*
    MIT License

    Copyright (c) 2025 Darek

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/

/*
 * ================== GLYPHGL UPDATE LOG ==================
 *
 * v1.0.0 | [Initial Release]
 * | - Custom TrueType parser and winding-rule rasterizer.
 * | - Zero-dependency GL loader (cross-platform).
 * | - Atlas-based rendering (GL_RED).
 *
 * v1.0.2 | 2025-10-28
 * | - Added 'glyph_renderer_update_projection' for handling window resize events.
 * | - Implemented text styling via bitmask: GLYPHGL_BOLD, GLYPHGL_UNDERLINE, GLYPHGL_ITALIC.
 * | - Optimized endianness conversions
 * | - Optimized contour decoding in 'glyph_ttf_get_glyph_bitmap' 
 * | - Optimized offset lookups in 'glyph_ttf__get_glyph_offset'
 * v1.0.3 | 2025-10-29
 * | - Added vertex batching as per the request from u/MGJared
 * | - Implemented custom memory allocation macros (GLYPH_MALLOC, GLYPH_FREE, GLYPH_REALLOC), for now relatively basic
 * v1.0.4 | 2025-10-29
 * | - Fixed memory fragmentation issue in glyph_renderer_draw_text by implementing a persistent vertex buffer
 * | - Replaced per-draw dynamic allocation with pre-allocated buffer that grows as needed
 * | - Reduces allocations from O(text_length) to O(1) for better performance in high-frequency rendering
 * v1.0.5 | 2025-10-30
 * | - Added 'GLYPHGL_DEBUG' macro to debug the library
 * | - Added 'GLYPH_LOG''
 * | - Created 'demos' and 'examples' folders
 * | - Added 'glyph_effect.h' that allows custom shader creation and includes many built in shaders
 * ========================================================
 */

#ifndef __GLYPH_H
#define __GLYPH_H

#include <stdlib.h>
#include "glyph_truetype.h"
#include "glyph_image.h"
#include "glyph_gl.h"
#include "glyph_util.h"
#include "glyph_effect.h"

#define GLYPHGL_BOLD        (1 << 0)
#define GLYPHGL_ITALIC      (1 << 1)
#define GLYPHGL_UNDERLINE   (1 << 2)


#define GLYPH_NONE           0
#define GLYPH_UTF8           0x010
#define GLYPH_ASCII          0x020

#include "glyph_atlas.h"


/*
    Default charsets for the atlas
*/
#define GLYPHGL_CHARSET_BASIC "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%%^&*()_+-=,./?|\n"
#define GLYPHGL_CHARSET_DEFAULT GLYPHGL_CHARSET_BASIC "€£¥¢₹₽±×÷√∫πΩ°∞≠≈≤≥∑∏∂∇∀∃∈∉⊂⊃∩∪←↑→↓"


int glyph_utf8_decode(const char* str, size_t* index);

typedef struct {
    glyph_atlas_t atlas;
    GLuint texture;
    GLuint shader;
    GLuint vao;
    GLuint vbo;
    float* vertex_buffer;
    size_t vertex_buffer_size;
    int initialized;
    uint32_t char_type;
    glyph_effect_t effect;
} glyph_renderer_t;


glyph_renderer_t glyph_renderer_create(const char* font_path, float pixel_height, const char* charset, uint32_t char_type, glyph_effect_t* effect);
void glyph_renderer_free(glyph_renderer_t* renderer);

void glyph_renderer_set_projection(glyph_renderer_t* renderer, int width, int height);

void glyph_renderer_update_projection(glyph_renderer_t* renderer, int width, int height);

void glyph_renderer_draw_text(glyph_renderer_t* renderer, const char* text, float x, float y, float scale,
    float r, float g, float b, int effects);

GLuint glyph_renderer_get_vao(glyph_renderer_t* renderer);

GLuint glyph_renderer_get_vbo(glyph_renderer_t* renderer);

GLuint glyph_renderer_get_shader(glyph_renderer_t* renderer);

int glyph_utf8_decode(const char* str, size_t* index);

#endif
