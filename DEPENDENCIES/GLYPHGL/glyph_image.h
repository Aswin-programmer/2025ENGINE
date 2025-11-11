/*
 * MIT License
 *
 * Copyright (c) 2025 Darek
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __GLYPH_IMAGE_h
#define __GLYPH_IMAGE_h

#include "glyph_util.h"

 // Cross-platform includes (Moved to C file where they are used, but kept here for type definitions)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

/**
 * @brief Structure representing a simple RGB image.
 *
 * The image data is stored as RGB values (three bytes per pixel) in row-major
 * order (left-to-right, top-to-bottom).
 */
typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned char* data;
} glyph_image_t;

/**
 * @brief Creates a new glyph image structure and allocates memory for its data.
 *
 * The image data is stored as RGB (3 bytes per pixel).
 *
 * @param width The width of the image.
 * @param height The height of the image.
 * @return A new glyph_image_t structure with allocated data.
 */
glyph_image_t glyph_image_create(unsigned int width, unsigned int height);

/**
 * @brief Frees the memory associated with a glyph image structure.
 *
 * @param img Pointer to the glyph_image_t structure to free.
 */
void glyph_image_free(glyph_image_t* img);

/**
 * @brief Computes the CRC32 checksum of the given data.
 *
 * @param data The data buffer.
 * @param len The length of the data buffer.
 * @return The CRC32 checksum.
 */
uint32_t crc32(const unsigned char* data, size_t len);

/**
 * @brief Computes the Adler32 checksum of the given data.
 *
 * @param data The data buffer.
 * @param len The length of the data buffer.
 * @return The Adler32 checksum.
 */
uint32_t adler32(const unsigned char* data, size_t len);

/**
 * @brief Writes the glyph image to a BMP file.
 *
 * The BMP format uses BGR order and writes rows from bottom to top,
 * and includes row padding.
 *
 * @param filename The path to the file to create.
 * @param img Pointer to the glyph_image_t to write.
 * @return 0 on success, -1 on failure.
 */
int glyph_write_bmp(const char* filename, glyph_image_t* img);

/**
 * @brief Writes the glyph image to a PNG file (uncompressed deflate).
 *
 * Writes the PNG signature, IHDR, compressed IDAT (uncompressed-stored deflate),
 * and IEND chunks.
 *
 * @param filename The path to the file to create.
 * @param img Pointer to the glyph_image_t to write.
 * @return 0 on success, -1 on failure.
 */
int glyph_write_png(const char* filename, glyph_image_t* img);

#endif // __GLYPH_IMAGE_h