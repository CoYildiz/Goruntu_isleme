#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assume these are defined in dip.cpp
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

// BMP File Header
typedef struct _BMPFH {
    WORD bfType;        // "BM" (0x4D42)
    DWORD bfSize;       // File size
    WORD bfReserved1;   // Reserved, usually 0
    WORD bfReserved2;   // Reserved, usually 0
    DWORD bfOffBits;    // Offset to pixel data
} BMPFH;

// BMP Info Header
typedef struct _BMPIH {
    DWORD biSize;          // Size of info header
    int biWidth;           // Image width
    int biHeight;          // Image height
    WORD biPlanes;         // Number of planes (usually 1)
    WORD biBitCount;       // Bits per pixel (24 for input, 8 for output)
    DWORD biCompression;   // Compression type
    DWORD biSizeImage;     // Size of image data
    int biXPelsPerMeter;   // Horizontal resolution
    int biYPelsPerMeter;   // Vertical resolution
    DWORD biClrUsed;       // Number of colors used (0 for 24-bit, 256 for 8-bit)
    DWORD biClrImportant;  // Number of important colors
} BMPIH;

// Palette structure
typedef struct _PALET {
    BYTE rgbBlue;       // Blue component (0-255)
    BYTE rgbGreen;      // Green component (0-255)
    BYTE rgbRed;        // Red component (0-255)
    BYTE rgbReserved;   // Reserved, usually 0
} PALET;

// Image structure
typedef struct _IMAGE {
    BMPFH bmpfh;        // File header
    BMPIH bmpih;        // Info header
    PALET* palet;       // Color palette (NULL for 24-bit)
    BYTE* data;         // Pixel data
} IMAGE;

// External functions from dip.cpp
extern IMAGE* read_bmp(const char* filename);
extern int write_bmp(const char* filename, IMAGE* img);
extern void writeInfo(IMAGE *image,char filename);
extern void free_bmp(IMAGE* img);

// Create a palette for a single color channel
void create_channel_palette(PALET* palette, char channel) {
    for (int i = 0; i < 256; i++) {
        palette[i].rgbReserved = 0;
        if (channel == 'R') {
            palette[i].rgbRed = i;
            palette[i].rgbGreen = 0;
            palette[i].rgbBlue = 0;
        } else if (channel == 'G') {
            palette[i].rgbRed = 0;
            palette[i].rgbGreen = i;
            palette[i].rgbBlue = 0;
        } else { // 'B'
            palette[i].rgbRed = 0;
            palette[i].rgbGreen = 0;
            palette[i].rgbBlue = i;
        }
    }
}

// Create an 8-bit image from a 24-bit image for a specific channel
IMAGE* create_channel_image(IMAGE* src, char channel) {
    if (!src || src->bmpih.biBitCount != 24) return NULL;

    int width = src->bmpih.biWidth;
    int height = abs(src->bmpih.biHeight);
    int row_padded_24 = (width * 3 + 3) & (~3); // Padded row size for 24-bit
    int row_padded_8 = (width + 3) & (~3);     // Padded row size for 8-bit
    int data_size_8 = row_padded_8 * height;

    // Allocate new IMAGE structure
    IMAGE* dst = (IMAGE*)malloc(sizeof(IMAGE));
    if (!dst) return NULL;

    // Copy and modify headers
    dst->bmpfh = src->bmpfh;
    dst->bmpih = src->bmpih;
    dst->bmpfh.bfOffBits = sizeof(BMPFH) + sizeof(BMPIH) + 256 * sizeof(PALET);
    dst->bmpfh.bfSize = dst->bmpfh.bfOffBits + data_size_8;
    dst->bmpih.biBitCount = 8;
    dst->bmpih.biClrUsed = 256;
    dst->bmpih.biSizeImage = data_size_8;

    // Allocate palette
    dst->palet = (PALET*)malloc(256 * sizeof(PALET));
    if (!dst->palet) {
        free(dst);
        return NULL;
    }
    create_channel_palette(dst->palet, channel);

    // Allocate pixel data
    dst->data = (BYTE*)malloc(data_size_8);
    if (!dst->data) {
        free(dst->palet);
        free(dst);
        return NULL;
    }

    // Extract channel data
    for (int y = 0; y < height; y++) {
        BYTE* src_row = src->data + y * row_padded_24;
        BYTE* dst_row = dst->data + y * row_padded_8;
        for (int x = 0; x < width; x++) {
            BYTE* src_pixel = src_row + x * 3;
            if (channel == 'R') {
                dst_row[x] = src_pixel[2]; // Red component
            } else if (channel == 'G') {
                dst_row[x] = src_pixel[1]; // Green component
            } else { // 'B'
                dst_row[x] = src_pixel[0]; // Blue component
            }
        }
    }

    return dst;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_24bit.bmp>\n", argv[0]);
        return 1;
    }

    // Read 24-bit BMP
    IMAGE* img = read_bmp(argv[1]);
    if (!img) {
        printf("Error: Could not read BMP file %s\n", argv[1]);
        return 1;
    }

    // Validate 24-bit format
    if (img->bmpih.biBitCount != 24) {
        printf("Error: Input file is not a 24-bit BMP\n");
        free_bmp(img);
        return 1;
    }

    // Create and save Red channel
    IMAGE* red_img = create_channel_image(img, 'R');
    if (!red_img || write_bmp("red8.bmp", red_img) != 0) {
        printf("Error: Failed to process or save red8.bmp\n");
        if (red_img) free_bmp(red_img);
        free_bmp(img);
        return 1;
    }
    free_bmp(red_img);

    // Create and save Green channel
    IMAGE* green_img = create_channel_image(img, 'G');
    if (!green_img || write_bmp("green8.bmp", green_img) != 0) {
        printf("Error: Failed to process or save green8.bmp\n");
        if (green_img) free_bmp(green_img);
        free_bmp(img);
        return 1;
    }
    free_bmp(green_img);

    // Create and save Blue channel
    IMAGE* blue_img = create_channel_image(img, 'B');
    if (!blue_img || write_bmp("blue8.bmp", blue_img) != 0) {
        printf("Error: Failed to process or save blue8.bmp\n");
        if (blue_img) free_bmp(blue_img);
        free_bmp(img);
        return 1;
    }
    free_bmp(blue_img);





    

    // Clean up
    free_bmp(img);
    printf("Successfully created red8.bmp, green8.bmp, and blue8.bmp\n");
    return 0;
}