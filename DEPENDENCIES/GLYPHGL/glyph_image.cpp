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

#include "glyph_image.h"

 // Cross-platform includes for implementations
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#endif
// Common includes (already included via "glyph_image.h" but explicit for clarity)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>


/*
    Creates a new glyph image structure setting its width and height and allocating memory for the image data
    which is stored as RGB values three bytes per pixel in row-major order from top to bottom
    quite a dumb fucking function to be honest
*/
glyph_image_t glyph_image_create(unsigned int width, unsigned int height) {
    glyph_image_t img;
    img.width = width;
    img.height = height;
    img.data = (unsigned char*)GLYPH_MALLOC((size_t)width * height * 3);
    return img;
}

void glyph_image_free(glyph_image_t* img) {
    if (!img) return;
    GLYPH_FREE(img->data);
    img->data = NULL;
}

// --- Checksum Utilities ---

static uint32_t crc32_table[256];
static int crc32_table_inited = 0;

/* Initializes a lookup table for CRC32 calculations if it hasn't been done already
    using the standard polynomial for IEEE 8023
*/
static void crc32_init_table(void) {
    if (crc32_table_inited) return;
    uint32_t poly = 0xEDB88320u;
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (int j = 0; j < 8; ++j) {
            if (crc & 1) crc = (crc >> 1) ^ poly;
            else crc >>= 1;
        }
        crc32_table[i] = crc;
    }
    crc32_table_inited = 1;
}

/*
    Computes the CRC32 checksum of the given data using the precomputed table
    it's a way to verify data integrity like a digital fingerprint that changes if even one bit is wrong
    useful for detecting corruption in files
*/
uint32_t crc32(const unsigned char* data, size_t len) {
    crc32_init_table();
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; ++i) {
        uint8_t idx = (uint8_t)(crc ^ data[i]);
        crc = (crc >> 8) ^ crc32_table[idx];
    }
    return crc ^ 0xFFFFFFFFu;
}

/*
    Computes the Adler32 checksum of the data which is a simpler and faster alternative to CRC32 for some uses
    it's like a quick hash that adds up the bytes in a clever way to catch errors
*/
uint32_t adler32(const unsigned char* data, size_t len) {
    const uint32_t MOD_ADLER = 65521u;
    uint32_t a = 1;
    uint32_t b = 0;
    for (size_t i = 0; i < len; ++i) {
        a = (a + data[i]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    return (b << 16) | a;
}


// --- File Writing Helpers ---

/*
Writes a 32-bit unsigned integer to a file in big-endian byte order meaning the most significant byte first
like how network protocols often send numbers to avoid endianness confusion
*/
static void write_u32_be(FILE* f, uint32_t v) {
    unsigned char b[4];
    b[0] = (v >> 24) & 0xFF;
    b[1] = (v >> 16) & 0xFF;
    b[2] = (v >> 8) & 0xFF;
    b[3] = v & 0xFF;
    fwrite(b, 1, 4, f);
}
/*
    Writes a 32-bit unsigned integer to a buffer in little-endian byte order least significant byte first
    like how Intel processors store numbers internally
*/
static void write_u32_le(unsigned char* buf, uint32_t v) {
    buf[0] = v & 0xFF;
    buf[1] = (v >> 8) & 0xFF;
    buf[2] = (v >> 16) & 0xFF;
    buf[3] = (v >> 24) & 0xFF;
}


// --- Image Writers ---

/*
    Writes the glyph image to a BMP file format which is a simple uncompressed bitmap
    it creates the file header and info header then writes the pixel data in BGR order bottom to top with padding to align rows
    like saving a screenshot but programmatically and without the print screen key
*/
int glyph_write_bmp(const char* filename, glyph_image_t* img) {
    if (!img || !img->data) return -1;
    FILE* f = fopen(filename, "wb");
    if (!f) return -1;

    int width = (int)img->width;
    int height = (int)img->height;
    // Row size must be a multiple of 4 bytes.
    int row_size = ((width * 3 + 3) / 4) * 4;
    int data_size = row_size * height;
    int file_size = 54 + data_size; // 14-byte File Header + 40-byte Info Header

    // BITMAPFILEHEADER (14 bytes)
    unsigned char fileheader[14] = {
        'B','M', // Signature
        (unsigned char)(file_size & 0xFF),
        (unsigned char)((file_size >> 8) & 0xFF),
        (unsigned char)((file_size >> 16) & 0xFF),
        (unsigned char)((file_size >> 24) & 0xFF),
        0,0, // Reserved 1
        0,0, // Reserved 2
        54,0,0,0 // Data offset (54 bytes)
    };
    fwrite(fileheader, 1, 14, f);

    // BITMAPINFOHEADER (40 bytes)
    unsigned char infoheader[40] = { 0 };
    infoheader[0] = 40; // Header size
    // Width (little-endian)
    write_u32_le(&infoheader[4], (uint32_t)width);
    // Height (little-endian)
    write_u32_le(&infoheader[8], (uint32_t)height);
    infoheader[12] = 1;     // Color planes
    infoheader[14] = 24;    // Bits per pixel (RGB)
    // Image size (little-endian)
    write_u32_le(&infoheader[20], (uint32_t)data_size);
    fwrite(infoheader, 1, 40, f);

    // Pixel Data (written bottom-to-top, BGR)
    for (int y = height - 1; y >= 0; y--) { // Iterate rows backwards (BMP is bottom-up)
        for (int x = 0; x < width; x++) {
            unsigned char r = img->data[(y * width + x) * 3 + 0];
            unsigned char g = img->data[(y * width + x) * 3 + 1];
            unsigned char b = img->data[(y * width + x) * 3 + 2];
            // Write B, then G, then R
            fputc(b, f);
            fputc(g, f);
            fputc(r, f);
        }
        // Write padding bytes
        int padding = (4 - (width * 3 % 4)) % 4;
        for (int p = 0; p < padding; p++) fputc(0, f);
    }

    fclose(f);
    return 0;
}

/*
    Writes the glyph image to a PNG file which is a compressed image format
    it builds the PNG structure with IHDR chunk for image info then compresses the raw pixel data using a simple stored deflate method
*/
int glyph_write_png(const char* filename, glyph_image_t* img) {
    if (!img || !img->data) return -1;
    FILE* f = fopen(filename, "wb");
    if (!f) return -1;

    // 1. Write PNG Signature
    const unsigned char png_sig[8] = { 137,80,78,71,13,10,26,10 };
    fwrite(png_sig, 1, 8, f);

    // 2. IHDR Chunk (Image Header)
    unsigned char ihdr_data[13];
    // Width (big-endian)
    ihdr_data[0] = (img->width >> 24) & 0xFF;
    ihdr_data[1] = (img->width >> 16) & 0xFF;
    ihdr_data[2] = (img->width >> 8) & 0xFF;
    ihdr_data[3] = img->width & 0xFF;
    // Height (big-endian)
    ihdr_data[4] = (img->height >> 24) & 0xFF;
    ihdr_data[5] = (img->height >> 16) & 0xFF;
    ihdr_data[6] = (img->height >> 8) & 0xFF;
    ihdr_data[7] = img->height & 0xFF;
    ihdr_data[8] = 8;    // Bit depth (8)
    ihdr_data[9] = 2;    // Color type (2: Truecolor, RGB)
    ihdr_data[10] = 0;   // Compression method (0: deflate)
    ihdr_data[11] = 0;   // Filter method (0: adaptive filtering)
    ihdr_data[12] = 0;   // Interlace method (0: none)

    write_u32_be(f, 13); // IHDR Data Length
    fwrite("IHDR", 1, 4, f); // Chunk Type
    fwrite(ihdr_data, 1, 13, f); // Chunk Data
    {
        // IHDR CRC
        unsigned char* tmp = (unsigned char*)GLYPH_MALLOC(4 + 13);
        memcpy(tmp, "IHDR", 4);
        memcpy(tmp + 4, ihdr_data, 13);
        uint32_t crc = crc32(tmp, 4 + 13);
        write_u32_be(f, crc);
        GLYPH_FREE(tmp);
    }

    // 3. IDAT Chunk (Image Data) - Simple Stored Deflate Implementation

    // Prepare "raw" data (scanlines with filter byte prepended)
    size_t raw_row_bytes = (size_t)img->width * 3 + 1; // 3 BPP + 1 filter byte
    size_t raw_size = raw_row_bytes * img->height;
    unsigned char* raw = (unsigned char*)GLYPH_MALLOC(raw_size);
    if (!raw) { fclose(f); return -1; }

    // Perform PNG filtering (Sub filter method is used: filter type 1)
    int bpp = 3;
    unsigned char* row_data = (unsigned char*)GLYPH_MALLOC((size_t)img->width * bpp);
    for (unsigned int y = 0; y < img->height; ++y) {
        unsigned char* row_ptr = raw + y * raw_row_bytes;
        // Copy current RGB data row
        memcpy(row_data, &img->data[(y * img->width) * bpp], (size_t)img->width * bpp);

        // Filter type 1: Sub (Pixel = Byte - Prior_Byte)
        row_ptr[0] = 1; // Filter Type Byte
        for (size_t i = 0; i < (size_t)img->width * bpp; ++i) {
            if (i < bpp) {
                // First 'bpp' bytes: no prior pixel (Prior_Byte is 0)
                row_ptr[i + 1] = row_data[i];
            }
            else {
                // Subsequent bytes: Subtract the value of the corresponding byte in the previous pixel
                row_ptr[i + 1] = row_data[i] - row_data[i - bpp];
            }
        }
    }
    GLYPH_FREE(row_data);

    // ZLIB Stream Construction (Deflate: Stored Blocks)
    unsigned char zlib_header[2] = { 0x78, 0x01 }; // CMF=78, FLG=01 (Fast compression)

    // Calculate max size for compressed data (header + data + blocks overhead + adler32)
    size_t max_blocks = (raw_size + 65534) / 65535;
    size_t comp_cap = 2 + raw_size + max_blocks * 5 + 4; // 2: ZLIB header, 5: Max block header, 4: Adler32
    unsigned char* comp = (unsigned char*)GLYPH_MALLOC(comp_cap);
    if (!comp) { GLYPH_FREE(raw); fclose(f); return -1; }

    size_t comp_pos = 0;
    // ZLIB Header
    comp[comp_pos++] = zlib_header[0];
    comp[comp_pos++] = zlib_header[1];

    // Deflate Blocks (Stored Blocks: BTYPE=00)
    size_t remaining = raw_size;
    size_t raw_pos = 0;
    while (remaining > 0) {
        uint16_t block_len = (uint16_t)((remaining > 65535) ? 65535 : remaining);
        uint8_t bfinal = (remaining <= 65535) ? 1 : 0; // Set final block flag

        // Deflate Block Header: BFINAL (1 bit) | BTYPE (2 bits) -> BTYPE=00 for stored
        uint8_t block_hdr = (uint8_t)((bfinal & 1) | (0 << 1));
        comp[comp_pos++] = block_hdr;

        // LEN (2 bytes, little-endian)
        comp[comp_pos++] = (uint8_t)(block_len & 0xFF);
        comp[comp_pos++] = (uint8_t)((block_len >> 8) & 0xFF);

        // NLEN (1's complement of LEN)
        uint16_t nlen = (uint16_t)(~block_len);
        comp[comp_pos++] = (uint8_t)(nlen & 0xFF);
        comp[comp_pos++] = (uint8_t)((nlen >> 8) & 0xFF);

        // Data block
        memcpy(comp + comp_pos, raw + raw_pos, block_len);
        comp_pos += block_len;
        raw_pos += block_len;
        remaining -= block_len;
    }

    // ZLIB Trailer (Adler32 Checksum)
    uint32_t a32 = adler32(raw, raw_size);
    // Write Adler32 (big-endian)
    comp[comp_pos++] = (unsigned char)((a32 >> 24) & 0xFF);
    comp[comp_pos++] = (unsigned char)((a32 >> 16) & 0xFF);
    comp[comp_pos++] = (unsigned char)((a32 >> 8) & 0xFF);
    comp[comp_pos++] = (unsigned char)(a32 & 0xFF);

    GLYPH_FREE(raw);

    // Write IDAT Chunk
    uint32_t comp_len = (uint32_t)comp_pos;
    write_u32_be(f, comp_len); // IDAT Data Length
    fwrite("IDAT", 1, 4, f); // Chunk Type
    fwrite(comp, 1, comp_len, f); // Chunk Data
    {
        // IDAT CRC
        unsigned char* tmp = (unsigned char*)GLYPH_MALLOC(4 + comp_len);
        memcpy(tmp, "IDAT", 4);
        memcpy(tmp + 4, comp, comp_len);
        uint32_t crc = crc32(tmp, 4 + comp_len);
        write_u32_be(f, crc);
        GLYPH_FREE(tmp);
    }
    GLYPH_FREE(comp);

    // 4. IEND Chunk (Image End)
    write_u32_be(f, 0); // IEND Data Length (0)
    fwrite("IEND", 1, 4, f); // Chunk Type
    uint32_t iend_crc = crc32((const unsigned char*)"IEND", 4);
    write_u32_be(f, iend_crc); // IEND CRC

    fclose(f);
    return 0;
}