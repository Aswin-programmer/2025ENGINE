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


#ifndef __GLYPH_TTF_H
#define __GLYPH_TTF_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "glyph_util.h"

typedef struct {
    unsigned char* data;
    int fontstart;
    int numGlyphs;
    int loca, head, glyf, hhea, hmtx, kern, gpos, cmap;
    int index_map;
    int indexToLocFormat;
    float scale;
} glyph_font_t;

typedef struct {
    int x0, y0, x1, y1;
    int advance, left_side_bearing;
} glyph_bbox_t;

typedef struct {
    float x, y;
    int on_curve;
} glyph_point_t;

int glyph_ttf_init(glyph_font_t* font, const unsigned char* data, int offset);
int glyph_ttf_find_glyph_index(const glyph_font_t* font, int codepoint);
void glyph_ttf_get_glyph_bbox(const glyph_font_t* font, int glyph_index, glyph_bbox_t* bbox);
unsigned char* glyph_ttf_get_glyph_bitmap(const glyph_font_t* font, int glyph_index, float scale_x, float scale_y, int* width, int* height, int* xoff, int* yoff);
void glyph_ttf_free_bitmap(unsigned char* bitmap);
float glyph_ttf_scale_for_pixel_height(const glyph_font_t* font, float pixels);
int glyph_ttf_get_glyph_advance(const glyph_font_t* font, int glyph_index);

static int glyph_ttf__isfont(const unsigned char* font);
static int glyph_ttf__find_table(const unsigned char* data, int fontstart, const char* tag);
static int glyph_ttf__get16(const unsigned char* data, int offset);
static unsigned int glyph_ttf__get16u(const unsigned char* data, int offset);
static int glyph_ttf__get32(const unsigned char* data, int offset);
static int glyph_ttf__get_glyph_offset(const glyph_font_t* font, int glyph_index);
static void glyph_ttf__rasterize_contour(unsigned char* bitmap, int w, int h, glyph_point_t* points, int n_points, float offset_x, float offset_y);
static void glyph_ttf__draw_line_aa(float* accum, int w, int h, float x0, float y0, float x1, float y1);
static void glyph_ttf__add_edge(float* accum, int w, int h, float x0, float y0, float x1, float y1);

#endif

#include "glyph_image.h"

static void glyph_ttf_debug_glyph(const glyph_font_t* font, int glyph_index);

static glyph_image_t glyph_ttf_render_glyph_to_image(const glyph_font_t* font, int glyph_index
    , float scale_x, float scale_y, unsigned char r, unsigned char g, unsigned char b);

int glyph_ttf_load_font_from_file(glyph_font_t* font, const char* filename);

void glyph_ttf_free_font(glyph_font_t* font);