#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#pragma pack(1)

typedef unsigned char BYTE; // 1 byte
typedef unsigned short WORD; // 2 bytes
typedef unsigned int DWORD; // 4 bytes
typedef int LONG; // 4 bytes

typedef struct _BMPFH // 14 bytes
{
    BYTE bftype1; // Must be 'B'
    BYTE bftype2; // Must be 'M'
    DWORD bfsize; // Size of BMP file (headers + palette + data)
    WORD bfreserved1; // Usually 0
    WORD bfreserved2; // Usually 0
    DWORD bfOffbits; // Offset to pixel data
} BMPFH;

typedef struct _BMPIH // 40 bytes
{
    DWORD bisize; // Size of info header
    LONG biw; // Width of image
    LONG bih; // Height of image
    WORD biplane; // Usually 1
    WORD bibitcount; // Bits per pixel (1, 4, 8, 24, etc.)
    DWORD biComp; // Compression type
    DWORD bisizeimage; // Size of image data
    LONG bix; // Horizontal resolution (not important)
    LONG biy; // Vertical resolution (not important)
    DWORD biclused; // Number of colors used
    DWORD biclimp; // Number of important colors
} BMPIH;

typedef struct _PALET
{
    BYTE rgbblue; // Blue component
    BYTE rgbgreen; // Green component
    BYTE rgbred; // Red component
    BYTE rgbreserved; // Reserved, usually 0
} PALET;

typedef struct _IMAGE
{
    BMPFH bmpfh;
    BMPIH bmpih;
    PALET *palet;
    BYTE *data;
} IMAGE;

IMAGE *ImageRead(IMAGE *image, char *filename)
{
    BMPFH bmpfh;
    BMPIH bmpih;
    FILE *fp;
    DWORD r, rowsize, size;
    fp = fopen(filename, "rb");
    if (fp == NULL) { printf("Hata: %s dosyasi bulunamadi\n", filename); exit(1); }
    fread(&bmpfh, sizeof(BMPFH), 1, fp);
    if (bmpfh.bftype1 != 'B' || bmpfh.bftype2 != 'M')
    {
        printf("Hata: %s bir BMP dosyasi degil\n", filename);
        fclose(fp);
        exit(1);
    }
    fread(&bmpih, sizeof(BMPIH), 1, fp);
    image = (IMAGE *)malloc(sizeof(IMAGE));
    if (image == NULL) { printf("Hata: Bellek ayrilamadi\n"); fclose(fp); exit(1); }
    image->bmpfh = bmpfh;
    image->bmpih = bmpih;
    
    r = 0;
    if (bmpih.bibitcount == 1) r = 2;
    if (bmpih.bibitcount == 4) r = 16;
    if (bmpih.bibitcount == 8) r = 256;
    if (r != 0)
    {
        image->palet = (PALET *)malloc(4 * r);
        if (image->palet == NULL) { printf("Hata: Palet icin bellek ayrilamadi\n"); free(image); fclose(fp); exit(1); }
        fread(image->palet, sizeof(BYTE), 4 * r, fp);
    }
    else
    {
        image->palet = NULL;
    }
    
    rowsize = (image->bmpih.biw * image->bmpih.bibitcount + 31) / 32 * 4;
    size = rowsize * abs(image->bmpih.bih);
    image->data = (BYTE *)malloc(size);
    if (image->data == NULL) { printf("Hata: Piksel verisi icin bellek ayrilamadi\n"); free(image->palet); free(image); fclose(fp); exit(1); }
    fread(image->data, size, 1, fp);
    fclose(fp);
    return image;
}

void ImageWrite(IMAGE *image, char *filename)
{
    FILE *fp;
    int r, rowsize, size;
    
    fp = fopen(filename, "wb");
    if (fp == NULL) { printf("Hata: %s dosyasi olusturulamadi\n", filename); exit(1); }
    fwrite(&image->bmpfh, sizeof(BMPFH), 1, fp);
    fwrite(&image->bmpih, sizeof(BMPIH), 1, fp);
    
    r = 0;
    if (image->bmpih.bibitcount == 1) r = 2;
    if (image->bmpih.bibitcount == 4) r = 16;
    if (image->bmpih.bibitcount == 8) r = 256;
    if (r != 0) fwrite(image->palet, 4 * r, 1, fp);
    
    rowsize = (image->bmpih.biw * image->bmpih.bibitcount + 31) / 32 * 4;
    size = rowsize * abs(image->bmpih.bih);
    fwrite(image->data, size, 1, fp);
    fclose(fp);
}

void writeInfo(IMAGE *image, char *fname)
{
    printf("--------Info about %s image file\n", fname);
    printf("bfType value        :%c%c\n", image->bmpfh.bftype1, image->bmpfh.bftype2);
    printf("bfsize             :%d\n", image->bmpfh.bfsize);
    printf("bfreserved1        :%d\n", image->bmpfh.bfreserved1);
    printf("bfreserved2        :%d\n", image->bmpfh.bfreserved2);
    printf("bfOffbits          :%d\n", image->bmpfh.bfOffbits);
    printf("bisize             :%d\n", image->bmpih.bisize);
    printf("Width              :%d\n", image->bmpih.biw);
    printf("Height             :%d\n", image->bmpih.bih);
    printf("biplane            :%d\n", image->bmpih.biplane);
    printf("bibitcount         :%d\n", image->bmpih.bibitcount);
    printf("Compression        :%d\n", image->bmpih.biComp);
    printf("bisizeimage        :%d\n", image->bmpih.bisizeimage);
    printf("bix                :%d\n", image->bmpih.bix);
    printf("biy                :%d\n", image->bmpih.biy);
    printf("bi color used      :%d\n", image->bmpih.biclused);
    printf("bi color imp.      :%d\n", image->bmpih.biclimp);
}

PALET* CreatePalette(int type)
{
    PALET* palet = (PALET*)malloc(256 * sizeof(PALET));
    if (!palet) {
        printf("Hata: Palet icin bellek ayrilamadi\n");
        exit(1);
    }

    int r = 0, g = 0, b = 0;
    if (type == 'r') r = 1;
    if (type == 'g') g = 1;
    if (type == 'b') b = 1;

    for (int i = 0; i < 256; i++) {
        palet[i].rgbRed = r * i;
        palet[i].rgbGreen = g * i;
        palet[i].rgbBlue = b * i;
        palet[i].rgbReserved = 0;
    }

    return palet;
}

IMAGE* CreateBitmap(BYTE* data, int width, int height)
{
    IMAGE* img = (IMAGE*)malloc(sizeof(IMAGE));
    if (!img) {
        printf("Hata: IMAGE yapisi icin bellek ayrilamadi\n");
        return NULL;
    }

    img->bmpfh.bfType1 = 'B';
    img->bmpfh.bfType2 = 'M';
    img->bmpfh.bfReserved1 = 0;
    img->bmpfh.bfReserved2 = 0;
    img->bmpfh.bfOffBits = sizeof(BMPFH) + sizeof(BMPIH) + 256 * sizeof(PALET);

    img->bmpih.bisize = sizeof(BMPIH);
    img->bmpih.biw = width;
    img->bmpih.bih = height;
    img->bmpih.biplane = 1;
    img->bmpih.bibitcount = 8;
    img->bmpih.biComp = 0;
    img->bmpih.bix = 0;
    img->bmpih.biy = 0;
    img->bmpih.biclused = 256;
    img->bmpih.biclimp = 0;

    int row_padded = (width + 31) / 32 * 4;
    img->bmpih.bisizeimage = row_padded * abs(height);
    img->bmpfh.bfsize = img->bmpfh.bfOffBits + img->bmpih.bisizeimage;

    img->data = (BYTE*)malloc(img->bmpih.bisizeimage);
    if (!img->data) {
        printf("Hata: Piksel verisi icin bellek ayrilamadi\n");
        free(img);
        return NULL;
    }

    for (int y = 0; y < abs(height); y++) {
        memcpy(img->data + y * row_padded, data + y * width, width);
    }

    img->palet = NULL; // Palette will be set by caller

    return img;
}

IMAGE* create_channel_image(IMAGE* src, char channel)
{
    if (!src || src->bmpih.bibitcount != 24) {
        printf("Hata: Gecersiz veya 24-bit olmayan giris\n");
        return NULL;
    }

    int width = src->bmpih.biw;
    int height = abs(src->bmpih.bih);
    int row_padded_24 = (width * 3 + 31) / 32 * 4;

    int row_padded_8 = (width + 31) / 32 * 4;
    int data_size_8 = row_padded_8 * height;
    BYTE* data = (BYTE*)malloc(data_size_8);
    if (!data) {
        printf("Hata: Piksel verisi icin bellek ayrilamadi\n");
        return NULL;
    }

    for (int y = 0; y < height; y++) {
        BYTE* src_row = src->data + y * row_padded_24;
        BYTE* dst_row = data + y * row_padded_8;
        for (int x = 0; x < width; x++) {
            BYTE* src_pixel = src_row + x * 3;
            if (channel == 'R') {
                dst_row[x] = src_pixel[2]; // Kırmızı
            } else if (channel == 'G') {
                dst_row[x] = src_pixel[1]; // Yeşil
            } else { // 'B'
                dst_row[x] = src_pixel[0]; // Mavi
            }
        }
    }

    IMAGE* dst = CreateBitmap(data, width, height);
    if (!dst) {
        free(data);
        return NULL;
    }

    dst->palet = CreatePalette(channel);
    if (!dst->palet) {
        free(dst->data);
        free(dst);
        free(data);
        return NULL;
    }

    free(data);
    return dst;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Kullanim: %s <input_24bit.bmp>\n", argv[0]);
        return 1;
    }

    IMAGE *image = (IMAGE*)malloc(sizeof(IMAGE));
    if (!image) {
        printf("Hata: Bellek ayrilamadi\n");
        return 1;
    }

    image = ImageRead(image, argv[1]);
    if (!image) {
        printf("Hata: %s dosyasi okunamadi\n", argv[1]);
        return 1;
    }

    writeInfo(image, argv[1]);

    if (image->bmpih.bibitcount != 24) {
        printf("Hata: Giris dosyasi 24-bit BMP degil\n");
        free(image->data);
        free(image->palet);
        free(image);
        return 1;
    }

    IMAGE* red_img = create_channel_image(image, 'R');
    if (!red_img || ImageWrite(red_img, "red8.bmp") != 0) {
        printf("Hata: red8.bmp olusturulamadi veya kaydedilemedi\n");
        if (red_img) {
            free(red_img->data);
            free(red_img->palet);
            free(red_img);
        }
        free(image->data);
        free(image->palet);
        free(image);
        return 1;
    }
    writeInfo(red_img, "red8.bmp");
    free(red_img->data);
    free(red_img->palet);
    free(red_img);

    IMAGE* green_img = create_channel_image(image, 'G');
    if (!green_img || ImageWrite(green_img, "green8.bmp") != 0) {
        printf("Hata: green8.bmp olusturulamadi veya kaydedilemedi\n");
        if (green_img) {
            free(green_img->data);
            free(green_img->palet);
            free(green_img);
        }
        free(image->data);
        free(image->palet);
        free(image);
        return 1;
    }
    writeInfo(green_img, "green8.bmp");
    free(green_img->data);
    free(green_img->palet);
    free(green_img);

    IMAGE* blue_img = create_channel_image(image, 'B');
    if (!blue_img || ImageWrite(blue_img, "blue8.bmp") != 0) {
        printf("Hata: blue8.bmp olusturulamadi veya kaydedilemedi\n");
        if (blue_img) {
            free(blue_img->data);
            free(blue_img->palet);
            free(blue_img);
        }
        free(image->data);
        free(image->palet);
        free(image);
        return 1;
    }
    writeInfo(blue_img, "blue8.bmp");
    free(blue_img->data);
    free(blue_img->palet);
    free(blue_img);

    free(image->data);
    free(image->palet);
    free(image);
    printf("red8.bmp, green8.bmp ve blue8.bmp basariyla olusturuldu\n");
    return 0;
}


