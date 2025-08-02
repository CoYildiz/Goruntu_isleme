
|                                                             |     |
| ----------------------------------------------------------- | --- |
| Bitmap File Header Information                              |     |
| Image Information                                           |     |
| Color Pallate(the 24 bits of more does not have)            |     |
| Data(Goruntuyu olusturan pixellerin tanimli oldugu alandir) |     |

   transformasyon(processing) **↓**

|                                              |
| -------------------------------------------- |
| Transformated Bitmap File Header Information |
|                                              |
|                                              |

- Pixelleri bellekten kapladigi alana gore sinirliyor($2^bit$ color)
- 1 pixel 1 bitlik goruntu = 1 bit 0 or 1 thus type of image has 2 colors
- 1 pixel 4 bitlik goruntu => 1,2,…,15 thus type of image has 16 colors
- 1 pixel 8 bitlik goruntu => 0,1,2,…,255 thus type of image has 256 colors
- 1 pixel 3 bytes this type of images has 224 colors( millions of color)the kind of images are called true color images

Pixel values typicaly represent gray levels,colours.heights.opacitices etc. Common image formats include:

- 1 samples per point(B&W or Grayscale)
- 3 samples per point(Red,Green,and Blue)
- 4 samples per point(Red,Green,Blue,and "Alpha", a.k.a Opacity)

DIP focuses on two major task
- Improvement of practorial information for human interpretation
- Processing of image data for storage,transmission and representition for autonomous machine perception

|                        |                    |                      |
| ---------------------- | ------------------ | -------------------- |
| Low Level process      | Mid Level Process  | High Level Process   |
| Input:Image            | Image              | Attributes           |
| Output:Image           | Attributes         | Understanding        |
| Examples:Noise removed | Object recognition | Score understanding, |
| ,image sharppening     | segmention         | auromous navigation  |
|                        |                    |                      |

Course will en

- # **BITMAP TUTORIAL (BMP)**

• A type of graphics format that specities a number of pixels and then specifies what color those pixels are GIF,JPG,and BMP etc. are bitmap images.

•Why BMP file
- simplicity
- Highly standardized
- Extremly widesprea//

The structure of a 24 bit bmp file:

- ## **The File Header:**
Holds information about type,size and layout of the file

-## **The info Header :**
Holds information about dimension and color format of the image

- ## **The image Data:**
hols pixel data and color table information

- ## **What BITMAP File Header**

|             |               |                               |
| ----------- | ------------- | ----------------------------- |
| Field Name  | Size in Bytes | Description                   |
| bfType      | 2             | Tthe charecters "BM"          |
| bfSize      | 4             | The size of the file in bytes |
| bfReserved1 | 2             | unused - must be zero         |
| bfReserved2 | 2             | unused - must be zero         |
| bfOffBits   | 4             | Offset to start of Pixel Data |

```c
`typedef struct{`   
	`unsigned short int type;`   
	`unsigned int size;`   
	`unsigned short int reserved1;`   
	`unsigned short int reserved2;`   
	`unsigned int offset;  }BITMAPFILEHEADERNN` 
	```


- ## **Bitmap Info Header**

|                 |               |                                                    |
| --------------- | ------------- | -------------------------------------------------- |
| Field Name      | Size in Bytes | Description                                        |
| biSize          | 4             | Header Size - Must be at least 40                  |
| biWidth         | 4             | Image width in pixels                              |
| biHeight        | 4             | Image height in pixels                             |
| biPlanes        | 2             | Must be 1                                          |
| biBitCount      | 2             | Bits per pixel - 1,2,…,16 or 32                    |
| biCompression   | 4             | Compression type(0 = uncompressed)                 |
| biSizeImage     | 4             | Image Size - may be zero for uncompressed imaged   |
| biXPelsPerMeter | 4             | Preferred resolution in pixels per meter(set to 0) |
| biYPelsPerMeter | 4             | Preferred resolution in pixels per meter(set to 0  |
| biClrUsed       | 4             | Number Color Map entries that are actually used    |
| biClrImportant  | 4             | Number of significant colors                       |

The compression types supported by BMP are listed below: 0 - no compression 1 - 8 bit run length encoding 2 - 4 bit run length encoding 3 - RGB bitmap with mask

```c
typedef struct{  
	unsigned int size;   
	int width, height;   
	unsigned short int planes;   
	unsigned short int bits;   
	unsigned int compression;   
	unsigned int imagesize;   
	int xresolution;   
	int yresolution;   
	unsigned int ncolours;   
	unsigned int importantcolours; }BITMAPINFOHEADER
```


- ## **Image Data**
    
```c
`typedef struct{`   
	`BYTE red;`   
	`BYTE green;`   
	`BYTE blue; }RGBTriplet;`  
```

RGBTriplet 
Bitmap_textureimage_Widthimage_height  OR,  GLubyte Bitmap_textureimage_Witdhtimage_heightimage_channels

# BMP Görüntü İşleme Kodu (C) ve Açıklama

Bu döküman, bir BMP görüntüsünü okuyup yazan C kodunu ve her bir bileşenin ayrıntılı açıklamasını içerir. Kod, bir BMP dosyasını okur, belleğe yükler ve başka bir dosyaya yazar. 

## C Kodu
```c
#include <stdio.h>
#include <stdlib.h>

// Temel veri tiplerini tanımlıyoruz, farklı platformlarda tutarlılık için
typedef unsigned char BYTE;   // 8 bitlik işaretsiz tamsayı
typedef unsigned short WORD;  // 16 bitlik işaretsiz tamsayı
typedef unsigned int DWORD;   // 32 bitlik işaretsiz tamsayı
typedef int LONG;            // 32 bitlik işaretli tamsayı

// BMP dosya yapısına uygun 1 bayt hizalama sağlıyoruz
#pragma pack(1)

// BMP Dosya Başlığı (File Header): BMP dosyasının genel bilgilerini tutar
typedef struct _BMPFH { // takes 14 bytes of memory
    BYTE bftype1;       // Dosya tipi, 'B' olmalı, 1 byte
    BYTE bftype2;       // Dosya tipi, 'M' olmalı, 1 byte
    DWORD bfsize;       // BMP dosyasının toplam bayt cinsinden boyutu, 4 bytes
    WORD bfreserved1;   // Genelde 0, rezerv ,2 bytes
    WORD bfreserved2;   // Genelde 0, rezerv ,2 bytes
    DWORD bfOffbits;    // Piksel verilerinin dosya içindeki başlangıç konumu , 4 bytes
} BMPFH;

// BMP Bilgi Başlığı (Info Header): Görüntünün özelliklerini tanımlar
typedef struct _BMPIH { //40 bytes 
    DWORD bisize;       // Bu başlığın boyutu 4 bytes
    LONG biw;           // Görüntü genişliği (piksel),width,4
    LONG bih;           // Görüntü yüksekliği (piksel),height,4
    WORD biplane;       // Renk düzlemi sayısı, genelde 1 , 2
    WORD bibitcount;    // Piksel başına bit sayısı (ör. 8 bit = 256 renk) , 2
    DWORD biComp;       // Sıkıştırma türü (0 = sıkıştırma yok), 4
    DWORD bisizeimage;  // Piksel verilerinin boyutu (bayt), 4
    LONG bix;           // Yatay çözünürlük (metrede piksel, genelde 0),4
    LONG biy;           // Dikey çözünürlük (metrede piksel, genelde 0),4
    DWORD biclused;     // Kullanılan renk sayısı (0 = hepsi),4
    DWORD biclimp;      // Önemli renk sayısı (genelde 0),4
} BMPIH;

// Renk Paleti: 8-bit veya daha düşük görüntüler için renk tablosu
typedef struct _PALET {
    BYTE rgbblue;       // Mavi bileşen (0-255)
    BYTE rgbgreen;      // Yeşil bileşen (0-255)
    BYTE rgbred;        // Kırmızı bileşen (0-255)
    BYTE rgbreserved;   // Rezerv, genelde 0
} PALET;

// Görüntü Yapısı: Tüm verileri (başlıklar, palet, piksel verileri) birleştirir
typedef struct _IMAGE {
    BMPFH bmpfh;        // Dosya başlığı
    BMPIH bmpih;        // Bilgi başlığı
    PALET* palet;       // Renk paleti (pointer, dinamik tahsis için)
    BYTE* data;         // Piksel verileri (pointer, dinamik tahsis için)
} IMAGE;

// BMP dosyasını okuyan fonksiyon
IMAGE* imageRead(IMAGE* image, const char* filename) {
    // Dosyayı ikili (binary) modda aç
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Dosya bulunamadı: %s\n", filename);
        exit(1);
    }

    // BMP dosya başlığını oku
    BMPFH bmpfh;
    fread(&bmpfh, sizeof(BMPFH), 1, file);
    // Dosyanın BMP formatında olduğunu kontrol et ('BM' imzası)
    if (bmpfh.bftype1 != 'B' || bmpfh.bftype2 != 'M') {
        fprintf(stderr, "Geçerli bir BMP dosyası değil.\n");
        fclose(file);
        exit(1);
    }

    // BMP bilgi başlığını oku
    BMPIH bmpih;
    fread(&bmpih, sizeof(BMPIH), 1, file);

    // IMAGE yapısı için bellek tahsis et
    image = (IMAGE*)malloc(sizeof(bmpfh.bfsize));
    if (!image) {
        fprintf(stderr, "Bellek tahsisi başarısız!\n");
        fclose(file);
        exit(1);
    }

    // Başlıkları IMAGE yapısına kopyala
    image->bmpfh = bmpfh;
    image->bmpih = bmpih;

    // Renk paleti boyutunu belirle (1-bit, 4-bit veya 8-bit görüntüler için)
    DWORD r = 0;
    if (bmpih.bibitcount == 1) r = 2;   // 1-bit: 2 renk
    if (bmpih.bibitcount == 4) r = 16;  // 4-bit: 16 renk
    if (bmpih.bibitcount == 8) r = 256; // 8-bit: 256 renk

    // Renk paleti varsa oku
    if (r != 0) {
        image->palet = (PALET*)malloc(4 * r); // Her renk 4 bayt (RGBA)
        if (!image->palet) {
            fprintf(stderr, "Palet için bellek tahsisi başarısız!\n");
            fclose(file);
            free(image);
            exit(1);
        }
        fread(image->palet,sizeof(BYTE), 4 * r, file);
    } else {
        image->palet = NULL; // Palet yoksa null yap
    }

    // Piksel verilerinin boyutunu hesapla
    // Satır boyutu: (genişlik * bit sayısı + 31) / 32 * 4 (4 bayt hizalaması için)
    DWORD rowsize = (image->bmpih.biw * image->bmpih.bibitcount + 31) / 32 * 4;
    DWORD size = rowsize * image->bmpih.bih; // Toplam veri boyutu

    // Piksel verileri için bellek tahsis et
    image->data = (BYTE*)malloc(size);
    if (!image->data) {
        fprintf(stderr, "Piksel verisi için bellek tahsisi başarısız!\n");
        fclose(file);
        free(image->palet);
        free(image);
        exit(1);
    }
    fread(image->data, size, 1, file);

    fclose(file); // Dosyayı kapat
    return image;
}

// BMP dosyasını yazan fonksiyon
void imageWrite(IMAGE* image, const char* filename) {
    // Dosyayı ikili (binary) modda aç
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Dosya açılamadı: %s\n", filename);
        exit(1);
    }

    // BMP dosya başlığını yaz
    fwrite(&image->bmpfh, sizeof(BMPFH), 1, file);

    // BMP bilgi başlığını yaz
    fwrite(&image->bmpih, sizeof(BMPIH), 1, file);

    // Renk paleti varsa yaz
    DWORD r = 0;
    if (image->bmpih.bibitcount == 1) r = 2;
    if (image->bmpih.bibitcount == 4) r = 16;
    if (image->bmpih.bibitcount == 8) r = 256;
    if (r != 0) {
        fwrite(image->palet, 4 * r, 1, file);
    }

    // Piksel verilerini yaz
    DWORD rowsize = (image->bmpih.biw * image->bmpih.bibitcount + 31) / 32 * 4;
    DWORD size = rowsize * image->bmpih.bih;
    fwrite(image->data, size, 1, file);

    fclose(file); // Dosyayı kapat
}

// Ana fonksiyon
int main() {
    IMAGE* image = (IMAGE*)malloc(sizeof(IMAGE)); // IMAGE yapısı için bellek tahsis
    if (!image) {
        fprintf(stderr, "Bellek tahsisi başarısız!\n");
        exit(1);
    }
    image = imageRead(image, "biber.bmp"); // Görüntüyü oku
    imageWrite(image, "prewitt.bmp");      // Görüntüyü yaz

    // Belleği serbest bırak
    free(image->data);
    if (image->palet) free(image->palet);
    free(image);

    return 0;
}
```

## Ayrıntılı Açıklama

### Genel Amaç

Bu kod, bir BMP görüntüsünü okumak ve başka bir BMP dosyasına yazmak için kullanılır. Şu anki haliyle, `biber.bmp` adında bir görüntüyü okur ve `prewitt.bmp` adıyla kopyalar. Herhangi bir görüntü işleme (ör. Prewitt filtresi) uygulamaz, ancak bu kod, görüntü işleme algoritmaları (filtreleme, kenar algılama) için temel bir altyapı sağlar.

### Kodun Bileşenleri

#### 1. Kütüphaneler

```c
#include <stdio.h>  // Dosya işlemleri (fopen, fread, fwrite) ve hata mesajları için
#include <stdlib.h> // malloc, free, exit gibi bellek yönetimi fonksiyonları için
```

- **Ne işe yarar?**: `stdio.h`, dosya okuma/yazma ve hata mesajları (`fprintf`) için gerekli. `stdlib.h`, dinamik bellek tahsisi (`malloc`, `free`) ve program sonlandırma (`exit`) için kullanılır.

#### 2. Veri Tipleri

```c
typedef unsigned char BYTE;   // 8 bitlik işaretsiz tamsayı
typedef unsigned short WORD;  // 16 bitlik işaretsiz tamsayı
typedef unsigned int DWORD;   // 32 bitlik işaretsiz tamsayı
typedef int LONG;            // 32 bitlik işaretli tamsayı
```

- **Ne işe yarar?**: BMP dosyaları, belirli boyutlarda veri tipleri kullanır. `typedef` ile bu tipler tanımlanarak kodun taşınabilirliği artar ve okunabilirlik sağlanır.
- **Neden önemli?**: BMP formatı, örneğin dosya boyutunu 32 bitlik işaretsiz tamsayı (`DWORD`) ile saklar. Yanlış tip kullanırsak, dosya yanlış okunur/yazılır.

#### 3. Hizalama

```c
#pragma pack(1)
```

- **Ne işe yarar?**: BMP dosyalarında veri yapıları 1 bayt hizalıdır. `#pragma pack(1)`, yapıların (struct) arasında boşluk (padding) eklenmesini önler.
- **Neden önemli?**: BMP dosyası, sıkı bir şekilde paketlenmiş veri içerir. Hizalama yanlış olursa, başlıklar yanlış okunur ve dosya bozulur.

#### 4. BMP Dosya Başlığı (`BMPFH`)

```c
typedef struct _BMPFH {
    BYTE bftype1;       // 'B'
    BYTE bftype2;       // 'M'
    DWORD bfsize;       // Dosya boyutu
    WORD bfreserved1;   // Rezerv, genelde 0
    WORD bfreserved2;   // Rezerv, genelde 0
    DWORD bfOffbits;    // Piksel verilerinin başlangıç konumu
} BMPFH;
```

- **Ne işe yarar?**: BMP dosyasının ilk kısmıdır (14 bayt). Dosyanın BMP formatında olduğunu (`'B'`, `'M'`) ve temel özelliklerini (boyut, piksel verisi konumu) tanımlar.
- **Örnek**: `bfOffbits`, piksel verilerinin dosya içinde kaçıncı bayttan başladığını söyler. Palet varsa, bu değer başlıklar + palet boyutunu kapsar.

#### 5. BMP Bilgi Başlığı (`BMPIH`)

```c
typedef struct _BMPIH {
    DWORD bisize;       // Başlık boyutu (40 bayt)
    LONG biw;           // Genişlik
    LONG bih;           // Yükseklik
    WORD biplane;       // Renk düzlemi, genelde 1
    WORD bibitcount;    // Piksel başına bit (8 = 256 renk)
    DWORD biComp;       // Sıkıştırma (0 = yok)
    DWORD bisizeimage;  // Piksel verisi boyutu
    LONG bix;           // Yatay çözünürlük
    LONG biy;           // Dikey çözünürlük
    DWORD biclused;     // Kullanılan renk sayısı
    DWORD biclimp;      // Önemli renk sayısı
} BMPIH;
```

- **Ne işe yarar?**: Görüntünün özelliklerini tanımlar (genelde 40 bayt). Genişlik, yükseklik, bit derinliği (ör. 8-bit = 256 renk) gibi bilgiler içerir.
- **Neden önemli?**: Görüntü işleme için bu bilgiler kritik. Örneğin, `bibitcount` 8 ise, her piksel bir palet indeksidir ve palet okunmalıdır.

#### 6. Renk Paleti (`PALET`)

```c
typedef struct _PALET {
    BYTE rgbblue;       // Mavi
    BYTE rgbgreen;      // Yeşil
    BYTE rgbred;        // Kırmızı
    BYTE rgbreserved;   // Rezerv, genelde 0
} PALET;
```

- **Ne işe yarar?**: 1-bit, 4-bit veya 8-bit görüntüler için renk tablosunu tanımlar. Her giriş, bir rengin RGB değerlerini saklar (4 bayt: RGBA).
- **Örnek**: 8-bit bir görüntüde 256 renkli palet olur. Her piksel, bu paletteki bir renge işaret eder.

#### 7. Görüntü Yapısı (`IMAGE`)

```c
typedef struct _IMAGE {
    BMPFH bmpfh;        // Dosya başlığı
    BMPIH bmpih;        // Bilgi başlığı
    PALET* palet;       // Renk paleti (pointer)
    BYTE* data;         // Piksel verileri (pointer)
} IMAGE;
```

- **Ne işe yarar?**: Tüm verileri (başlıklar, palet, pikseller) tek bir yapıda birleştirir. `palet` ve `data` pointer’ları, dinamik bellek tahsisi için kullanılır.
- **Neden pointer?**: Palet ve piksel verileri, görüntü boyutuna bağlı olarak değişken boyutta. Bu yüzden dinamik tahsis (`malloc`) gerekir.

#### 8. `imageRead` Fonksiyonu

```c
IMAGE* imageRead(IMAGE* image, const char* filename)
```

- **Ne işe yarar?**: Bir BMP dosyasını okur ve verilerini `IMAGE` yapısına yükler. Geri dönüş değeri, tahsis edilen `IMAGE` pointer’ıdır.
- **Açıklama**:
    - Dosyayı ikili modda (`"rb"`) açar.
    - Dosya başlığını (`BMPFH`) okur ve `'BM'` imzasını kontrol eder.
    - Bilgi başlığını (`BMPIH`) okur.
    - `IMAGE` yapısı için bellek tahsis eder ve başlıkları kopyalar.
    - Bit derinliğine göre palet boyutunu belirler (1-bit = 2 renk, 4-bit = 16 renk, 8-bit = 256 renk) ve paleti okur.
    - Piksel verilerinin boyutunu hesaplar (her satır 4 bayta hizalanır) ve verileri okur.
    - Dosyayı kapatır ve `IMAGE` pointer’ını döndürür.

#### 9. `imageWrite` Fonksiyonu

```c
void imageWrite(IMAGE* image, const char* filename)
```

- **Ne işe yarar?**: `IMAGE` yapısındaki verileri bir BMP dosyasına yazar.
- **Açıklama**:
    - Dosyayı ikili yazma modunda (`"wb"`) açar.
    - Dosya ve bilgi başlıklarını yazar.
    - Palet varsa, dosyaya yazar.
    - Piksel verilerini yazar (satır boyutu hizalaması dikkate alınır).
    - Dosyayı kapatır.

#### 10. `main` Fonksiyonu

```c
int main()
```

- **Ne işe yarar?**: Programın giriş noktası. `biber.bmp` dosyasını okur, `prewitt.bmp` dosyasına yazar ve belleği temizler.
- **Açıklama**:
    - `IMAGE` yapısı için bellek tahsis eder.
    - `imageRead` ile görüntüyü okur.
    - `imageWrite` ile görüntüyü yazar.
    - Belleği (`data`, `palet`, `image`) serbest bırakır.

## Genel Değerlendirme

- **Ne yapıyor?**: Kod, bir BMP görüntüsünü okuyor, belleğe yüklüyor ve başka bir dosyaya yazıyor. Şu an herhangi bir işlem (ör. Prewitt filtresi) uygulamıyor, sadece kopyalama yapıyor.
- **Neden önemli?**: Bu, düşük seviyeli görüntü işleme için temel bir yapı. Örneğin, `image->data` üzerinde döngülerle piksel manipülasyonu (filtreleme, kenar algılama) yapabilirsiniz.
- **Pointer’ların rolü**: `image->palet` ve `image->data`, dinamik bellek tahsisi için pointer kullanıyor. Bu, değişken boyutlu verileri (palet ve pikseller) işlemek için şart.

# IMAGE yapısının bilgilerini konsola yazdıran fonksiyon
Bu kisimda bmp dosyasi ile ilgili bilgi veren fonksiyon yaziyoruz main() icerisine okuduktan sonra yani imageRead fonksiyonundan sonra writeInfo(image,name.bmp) yazilarak erisilebilir.

image = imageRead(image, "biber.bmp");
writeInfo(image, "biber.bmp"); // Bilgileri konsola yaz, dosya adıyla
imageWrite(image, "prewitt.bmp");


```c
void writeInfo(IMAGE* image,char fname) {//fname neden tanimladik anlamadim
// Hata kontrolü: image pointer'ı geçerli mi?
if (!image) {
fprintf(stderr, "Hata: Geçersiz IMAGE pointer'ı!\n");
return;
}

// BMP Dosya Başlığı (BMPFH) bilgilerini yazdır
printf("BMP Dosya Başlığı (File Header):\n");
printf(" Dosya Tipi: %c%c\n", image->bmpfh.bftype1, image->bmpfh.bftype2);
printf(" Dosya Boyutu: %u bayt\n", image->bmpfh.bfsize);
printf(" Rezerv 1: %u\n", image->bmpfh.bfreserved1);
printf(" Rezerv 2: %u\n", image->bmpfh.bfreserved2);
printf(" Piksel Verisi Başlangıcı: %u bayt\n", image->bmpfh.bfOffbits);

// BMP Bilgi Başlığı (BMPIH) bilgilerini yazdır
printf("\nBMP Bilgi Başlığı (Info Header):\n");
printf(" Başlık Boyutu: %u bayt\n", image->bmpih.bisize);
printf(" Görüntü Genişliği: %d piksel\n", image->bmpih.biw);
printf(" Görüntü Yüksekliği: %d piksel\n", image->bmpih.bih);
printf(" Renk Düzlemi Sayısı: %u\n", image->bmpih.biplane);
printf(" Piksel Başına Bit: %u\n", image->bmpih.bibitcount);
printf(" Sıkıştırma Türü: %u (%s)\n", image->bmpih.biComp,
image->bmpih.biComp == 0 ? "Yok" : "Var");
printf(" Piksel Verisi Boyutu: %u bayt\n", image->bmpih.bisizeimage);
printf(" Yatay Çözünürlük: %d piksel/metre\n", image->bmpih.bix);
printf(" Dikey Çözünürlük: %d piksel/metre\n", image->bmpih.biy);
printf(" Kullanılan Renk Sayısı: %u\n", image->bmpih.biclused);
printf(" Önemli Renk Sayısı: %u\n", image->bmpih.biclimp);

// Renk paleti bilgisi
DWORD r = 0;
if (image->bmpih.bibitcount == 1) r = 2;
if (image->bmpih.bibitcount == 4) r = 16;
if (image->bmpih.bibitcount == 8) r = 256;
printf("\nRenk Paleti:\n");
if (r != 0 && image->palet) {
printf(" Palet Boyutu: %u renk\n", r);

// İlk birkaç rengi örnek olarak yazdır
for (DWORD i = 0; i < r && i < 5; i++) { // Maksimum 5 renk göster
printf(" Renk %u: RGB(%u, %u, %u)\n", i,
image->palet[i].rgbred,
image->palet[i].rgbgreen,
image->palet[i].rgbblue);
}
if (r > 5) printf(" ... ve %u renk daha\n", r - 5);
} else {
printf(" Palet Yok\n");
	}
}
```