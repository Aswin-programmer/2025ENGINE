/*
 * MIT License
 * * Copyright (c) 2025 Darek
 * * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#define GLYPHGL_BOLD        (1 << 0)
#define GLYPHGL_ITALIC      (1 << 1)
#define GLYPHGL_UNDERLINE   (1 << 2)


#define GLYPH_NONE           0
#define GLYPH_UTF8           0x010
#define GLYPH_ASCII          0x020

#ifndef GLYPH_ATLAS_H
#define GLYPH_ATLAS_H

// Assume these headers exist and define types like glyph_image_t, GLYPH_LOG, etc.
#include "glyph_image.h"
#include "glyph_util.h"
#include "glyph_truetype.h"
// We only need basic types/functions in the header. System includes are mostly for the implementation.
#include <stddef.h>
#include <stdint.h>
// Note: We avoid including heavy system headers (like windows.h or math.h) in the header
// unless absolutely necessary for type declarations.

#ifdef __cplusplus
extern "C" {
#endif

    // --- Type Definitions ---

    // Structure holding metrics for a single character in the atlas
    typedef struct {
        int codepoint;
        int x, y;          // Position in the atlas texture
        int width, height; // Size of the glyph bitmap
        int xoff, yoff;    // Bearing/offset from the pen position
        int advance;       // Horizontal advance for the next character
    } glyph_atlas_char_t;

    // Structure representing the final font atlas
    typedef struct {
        glyph_image_t image;          // The texture data (bitmap)
        glyph_atlas_char_t* chars;    // Array of character metrics
        int num_chars;                // Number of characters stored
        float pixel_height;           // Font size in pixels (height)
    } glyph_atlas_t;

    // --- Helper Functions (Prototypes) ---

    // Decodes a UTF-8 sequence starting at *index from str, updates *index, and returns the codepoint.
    // This function is static in the original file, so it's kept static here, meaning it's only visible
    // in the compilation unit (the .cpp file) that includes it.
    static int glyph_atlas_utf8_decode(const char* str, size_t* index);

    // Helper to calculate the next power of 2. Static in the original file.
    static int glyph_atlas__next_pow2(int v);


    // --- Public API Function Prototypes ---

    // Creates and populates a new font atlas structure.
    // Requires GLYPH_UTF8 or another char_type constant from glyph_util.h.
    glyph_atlas_t glyph_atlas_create(const char* font_path, float pixel_height, const char* charset, uint32_t char_type);

    // Frees all resources associated with the atlas (image data and character metrics).
    void glyph_atlas_free(glyph_atlas_t* atlas);

    // Saves the atlas image data to a PNG file.
    int glyph_atlas_save_png(glyph_atlas_t* atlas, const char* output_path);

    // Saves the atlas image data to a BMP file.
    int glyph_atlas_save_bmp(glyph_atlas_t* atlas, const char* output_path);

    // Saves the character metadata to a text file.
    int glyph_atlas_save_metadata(glyph_atlas_t* atlas, const char* output_path);

    // Finds the character metrics for a given Unicode codepoint.
    glyph_atlas_char_t* glyph_atlas_find_char(glyph_atlas_t* atlas, int codepoint);

    // Prints information about the atlas to the console/log.
    void glyph_atlas_print_info(glyph_atlas_t* atlas);


#ifdef __cplusplus
}
#endif

#endif // GLYPH_ATLAS_H