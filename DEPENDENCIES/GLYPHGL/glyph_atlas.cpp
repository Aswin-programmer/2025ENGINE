#include "glyph_atlas.h"

// --- System Includes for Implementation ---
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h> // For sqrtf

// Assume these headers contain necessary definitions (like glyph_font_t and its functions)
// Since the original file used them, we assume they're available.
// NOTE: We rely on "glyph_util.h" (included in .h) to define GLYPH_MALLOC, GLYPH_FREE, and GLYPH_LOG.
// NOTE: We rely on "glyph_image.h" (included in .h) to define glyph_image_t, glyph_image_create, glyph_image_free.
// NOTE: We rely on an external module (not shown in the original code, but implied by usage) for font loading:
//   - glyph_font_t
//   - glyph_ttf_load_font_from_file
//   - glyph_ttf_scale_for_pixel_height
//   - glyph_ttf_find_glyph_index
//   - glyph_ttf_get_glyph_bitmap
//   - glyph_ttf_get_glyph_advance
//   - glyph_ttf_free_font
//   - glyph_ttf_free_bitmap
//   - glyph_write_png
//   - glyph_write_bmp

// Assuming the necessary function definitions from the TTF/Image modules are linked in.

// --- Static Helper Function Definitions ---

static int glyph_atlas_utf8_decode(const char* str, size_t* index) {
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
    return 0xFFFD; // Unicode replacement character
}

static int glyph_atlas__next_pow2(int v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

// --- Internal Helper Structure for Glyph Bitmap Temporarily ---

typedef struct {
    unsigned char* bitmap;
    int width, height;
    int xoff, yoff;
    int advance;
    int is_default;
} temp_glyph_t;


// --- Public API Function Definitions ---

glyph_atlas_t glyph_atlas_create(const char* font_path, float pixel_height, const char* charset, uint32_t char_type) {
    glyph_atlas_t atlas = { 0 };
    // The font type must be defined in one of the included headers (e.g., glyph_util.h)
    // Assuming 'glyph_font_t' is a type from an external TTF module
    glyph_font_t font;

    if (!glyph_ttf_load_font_from_file(&font, font_path)) {
        GLYPH_LOG("Failed to load font: %s\n", font_path);
        return atlas;
    }

    float scale = glyph_ttf_scale_for_pixel_height(&font, pixel_height);
    atlas.pixel_height = pixel_height;

    // Default charset if none is provided
    if (!charset) {
        // The original code uses a repeated string literal, which suggests ASCII range.
        charset = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    }

    int charset_len;
    if (char_type == GLYPH_UTF8) {
        // Calculate the number of codepoints in the UTF-8 string
        charset_len = 0;
        size_t idx = 0;
        while (idx < strlen(charset)) {
            glyph_atlas_utf8_decode(charset, &idx);
            charset_len++;
        }
    }
    else {
        charset_len = strlen(charset);
    }

    atlas.num_chars = charset_len;
    atlas.chars = (glyph_atlas_char_t*)GLYPH_MALLOC(charset_len * sizeof(glyph_atlas_char_t));
    if (!atlas.chars) {
        glyph_ttf_free_font(&font);
        return atlas;
    }

    temp_glyph_t* temp_glyphs = (temp_glyph_t*)GLYPH_MALLOC(charset_len * sizeof(temp_glyph_t));
    if (!temp_glyphs) {
        GLYPH_FREE(atlas.chars);
        atlas.chars = NULL;
        glyph_ttf_free_font(&font);
        return atlas;
    }

    int total_width = 0;
    int max_height = 0;

    size_t charset_idx = 0;
    for (int i = 0; i < charset_len; i++) {
        int codepoint;
        if (char_type == GLYPH_UTF8) {
            codepoint = glyph_atlas_utf8_decode(charset, &charset_idx);
        }
        else {
            codepoint = (unsigned char)charset[i];
        }
        int glyph_idx = glyph_ttf_find_glyph_index(&font, codepoint);

        // Handle space or un-renderable glyphs
        if (glyph_idx == 0 && codepoint != ' ') {
            temp_glyphs[i].bitmap = NULL;
            temp_glyphs[i].width = 0;
            temp_glyphs[i].height = 0;
            temp_glyphs[i].xoff = 0;
            temp_glyphs[i].yoff = 0;
            temp_glyphs[i].advance = (int)(pixel_height * 0.5f); // Estimate advance
            temp_glyphs[i].is_default = 0;
            atlas.chars[i].codepoint = codepoint;
            atlas.chars[i].advance = temp_glyphs[i].advance;
            continue;
        }

        int width, height, xoff, yoff;
        unsigned char* bitmap = glyph_ttf_get_glyph_bitmap(&font, glyph_idx, scale, scale,
            &width, &height, &xoff, &yoff);

        temp_glyphs[i].bitmap = bitmap;
        temp_glyphs[i].width = width;
        temp_glyphs[i].height = height;
        temp_glyphs[i].xoff = xoff;
        temp_glyphs[i].yoff = yoff;
        temp_glyphs[i].advance = (int)(glyph_ttf_get_glyph_advance(&font, glyph_idx) * scale);
        temp_glyphs[i].is_default = 0;

        atlas.chars[i].codepoint = codepoint;
        atlas.chars[i].advance = temp_glyphs[i].advance;

        total_width += width + 4; // Add padding
        if (height > max_height) {
            max_height = height;
        }
    }

    // Calculate initial atlas size
    int padding = 4;
    int estimated_min_size = (int)sqrtf(total_width * max_height) + 256;
    int atlas_width = glyph_atlas__next_pow2(estimated_min_size);
    int atlas_height = atlas_width;

    // Ensure minimum size
    if (atlas_width < 2048) atlas_width = 2048;
    if (atlas_height < 2048) atlas_height = 2048;

    // Create the image
    atlas.image = glyph_image_create(atlas_width, atlas_height);
    memset(atlas.image.data, 0, atlas_width * atlas_height * 3); // Initialize with black/transparent

    int pen_x = padding;
    int pen_y = padding;
    int row_height = 0;

    typedef struct {
        int index;
        int yoff;
    } row_glyph_t;

    // Use a dynamic array to track glyphs in the current row
    row_glyph_t* current_row = (row_glyph_t*)GLYPH_MALLOC(charset_len * sizeof(row_glyph_t));
    int row_count = 0;

    for (int i = 0; i < charset_len; i++) {
        // Skip un-renderable or space characters
        if (!temp_glyphs[i].bitmap || temp_glyphs[i].width == 0) {
            // Ensure char metrics are initialized (even if width/height is 0)
            atlas.chars[i].x = 0;
            atlas.chars[i].y = 0;
            atlas.chars[i].width = 0;
            atlas.chars[i].height = 0;
            atlas.chars[i].xoff = 0;
            atlas.chars[i].yoff = 0;
            continue;
        }

        // Check if the glyph fits in the current row
        if (pen_x + temp_glyphs[i].width + padding > atlas_width) {
            // The row is full, render previous row's glyphs
            int max_yoff = 0;
            for (int r = 0; r < row_count; r++) {
                if (temp_glyphs[current_row[r].index].yoff > max_yoff) {
                    max_yoff = temp_glyphs[current_row[r].index].yoff;
                }
            }

            // Finalize Y position and blit all glyphs in the row, aligning by baseline (max_yoff)
            for (int r = 0; r < row_count; r++) {
                int idx = current_row[r].index;
                // Calculate the top edge of the glyph relative to the row's baseline (pen_y + max_yoff)
                int glyph_top_offset = max_yoff - temp_glyphs[idx].yoff;

                // Blit the bitmap data to the atlas
                for (int y = 0; y < temp_glyphs[idx].height; y++) {
                    for (int x = 0; x < temp_glyphs[idx].width; x++) {
                        int atlas_x = atlas.chars[idx].x + x;
                        // y coordinate is adjusted for baseline alignment
                        int atlas_y = atlas.chars[idx].y + glyph_top_offset + y;
                        unsigned char alpha = temp_glyphs[idx].bitmap[y * temp_glyphs[idx].width + x];

                        // Store in the R, G, B channels (creating a grayscale image for R-only texture sampling)
                        int pixel_idx = (atlas_y * atlas_width + atlas_x) * 3;
                        atlas.image.data[pixel_idx + 0] = alpha;
                        atlas.image.data[pixel_idx + 1] = alpha;
                        atlas.image.data[pixel_idx + 2] = alpha;
                    }
                }

                // Update the final Y position for the char metrics
                atlas.chars[idx].y = atlas.chars[idx].y + glyph_top_offset;
            }

            // Start a new row
            pen_x = padding;
            pen_y += row_height + padding;
            row_height = 0;
            row_count = 0;
        }

        // Check if the current glyph fits vertically in the atlas
        if (pen_y + temp_glyphs[i].height + padding > atlas_height) {
            // Atlas is too small. Double the size and restart the packing process.
            GLYPH_LOG("Warning: Atlas too small, increasing size for glyph %d\n", i);
            atlas_width *= 2;
            atlas_height *= 2;
            glyph_image_free(&atlas.image);
            atlas.image = glyph_image_create(atlas_width, atlas_height);
            memset(atlas.image.data, 0, atlas_width * atlas_height * 3);

            // Reset packing state and restart the loop from the beginning (i = -1)
            pen_x = padding;
            pen_y = padding;
            row_height = 0;
            row_count = 0;
            i = -1;
            continue;
        }

        // Add the current glyph to the current row list
        current_row[row_count].index = i;
        current_row[row_count].yoff = temp_glyphs[i].yoff;
        row_count++;

        // Update temporary metrics in atlas.chars
        atlas.chars[i].x = pen_x;
        atlas.chars[i].y = pen_y; // Temporary Y pos (will be finalized after row wrap)
        atlas.chars[i].width = temp_glyphs[i].width;
        atlas.chars[i].height = temp_glyphs[i].height;
        atlas.chars[i].xoff = temp_glyphs[i].xoff;
        atlas.chars[i].yoff = temp_glyphs[i].yoff; // yoff is needed for baseline alignment

        // Update current row height
        int bottom = temp_glyphs[i].height;
        row_height = (bottom > row_height) ? bottom : row_height;

        // Advance pen position
        pen_x += temp_glyphs[i].width + padding * 2;
    }

    // Render the final (last) row if any glyphs remain
    if (row_count > 0) {
        int max_yoff = 0;
        for (int r = 0; r < row_count; r++) {
            if (temp_glyphs[current_row[r].index].yoff > max_yoff) {
                max_yoff = temp_glyphs[current_row[r].index].yoff;
            }
        }

        for (int r = 0; r < row_count; r++) {
            int idx = current_row[r].index;
            int glyph_top_offset = max_yoff - temp_glyphs[idx].yoff;

            for (int y = 0; y < temp_glyphs[idx].height; y++) {
                for (int x = 0; x < temp_glyphs[idx].width; x++) {
                    int atlas_x = atlas.chars[idx].x + x;
                    int atlas_y = atlas.chars[idx].y + glyph_top_offset + y;
                    unsigned char alpha = temp_glyphs[idx].bitmap[y * temp_glyphs[idx].width + x];

                    int pixel_idx = (atlas_y * atlas_width + atlas_x) * 3;
                    atlas.image.data[pixel_idx + 0] = alpha;
                    atlas.image.data[pixel_idx + 1] = alpha;
                    atlas.image.data[pixel_idx + 2] = alpha;
                }
            }

            atlas.chars[idx].y = atlas.chars[idx].y + glyph_top_offset;
        }
    }

    // Cleanup temporary resources
    GLYPH_FREE(current_row);

    for (int i = 0; i < charset_len; i++) {
        if (temp_glyphs[i].bitmap) {
            if (temp_glyphs[i].is_default) {
                GLYPH_FREE(temp_glyphs[i].bitmap);
            }
            else {
                // Assuming glyph_ttf_free_bitmap is defined externally
                glyph_ttf_free_bitmap(temp_glyphs[i].bitmap);
            }
        }
    }
    GLYPH_FREE(temp_glyphs);

    // Assuming glyph_ttf_free_font is defined externally
    glyph_ttf_free_font(&font);

    return atlas;
}

void glyph_atlas_free(glyph_atlas_t* atlas) {
    if (!atlas) return;
    if (atlas->chars) {
        GLYPH_FREE(atlas->chars);
        atlas->chars = NULL;
    }
    // Assuming glyph_image_free is defined externally
    glyph_image_free(&atlas->image);
    atlas->num_chars = 0;
}

int glyph_atlas_save_png(glyph_atlas_t* atlas, const char* output_path) {
    if (!atlas || !atlas->image.data) return -1;
    // Assuming glyph_write_png is defined externally
    return glyph_write_png(output_path, &atlas->image);
}

int glyph_atlas_save_bmp(glyph_atlas_t* atlas, const char* output_path) {
    if (!atlas || !atlas->image.data) return -1;
    // Assuming glyph_write_bmp is defined externally
    return glyph_write_bmp(output_path, &atlas->image);
}

int glyph_atlas_save_metadata(glyph_atlas_t* atlas, const char* output_path) {
    if (!atlas || !atlas->chars) return -1;

    FILE* f = fopen(output_path, "w");
    if (!f) return -1;

    fprintf(f, "# Font Atlas Metadata\n");
    fprintf(f, "pixel_height: %.2f\n", atlas->pixel_height);
    // Use %u for unsigned int width/height (assuming glyph_image_t uses unsigned for dimensions)
    fprintf(f, "atlas_width: %u\n", atlas->image.width);
    fprintf(f, "atlas_height: %u\n", atlas->image.height);
    fprintf(f, "num_chars: %d\n\n", atlas->num_chars);
    fprintf(f, "# codepoint x y width height xoff yoff advance\n");

    for (int i = 0; i < atlas->num_chars; i++) {
        glyph_atlas_char_t* c = &atlas->chars[i];
        fprintf(f, "%d %d %d %d %d %d %d %d\n",
            c->codepoint, c->x, c->y, c->width, c->height,
            c->xoff, c->yoff, c->advance);
    }

    fclose(f);
    return 0;
}

glyph_atlas_char_t* glyph_atlas_find_char(glyph_atlas_t* atlas, int codepoint) {
    if (!atlas || !atlas->chars) return NULL;

    // Linear search is generally acceptable for font atlases as the number of characters is typically limited
    for (int i = 0; i < atlas->num_chars; i++) {
        if (atlas->chars[i].codepoint == codepoint) {
            return &atlas->chars[i];
        }
    }
    return NULL;
}

void glyph_atlas_print_info(glyph_atlas_t* atlas) {
    if (!atlas) return;

    GLYPH_LOG("Font Atlas Info:\n");
    GLYPH_LOG("  Atlas Size: %ux%u\n", atlas->image.width, atlas->image.height);
    GLYPH_LOG("  Pixel Height: %.2f\n", atlas->pixel_height);
    GLYPH_LOG("  Characters: %d\n", atlas->num_chars);
    GLYPH_LOG("\nCharacter Details:\n");

    for (int i = 0; i < atlas->num_chars; i++) {
        glyph_atlas_char_t* c = &atlas->chars[i];
        // Safely display the character if it's within the printable ASCII range
        char ch = (c->codepoint >= 32 && c->codepoint < 127) ? (char)c->codepoint : '?';
        GLYPH_LOG("  '%c' (U+%04X): pos=(%d,%d) size=(%dx%d) offset=(%d,%d) advance=%d\n",
            ch, c->codepoint, c->x, c->y, c->width, c->height,
            c->xoff, c->yoff, c->advance);
    }
}