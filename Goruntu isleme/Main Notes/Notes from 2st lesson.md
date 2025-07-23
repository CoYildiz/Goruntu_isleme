## A small program for writing to binary files
```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE *fp;
  int x = 123456;
  double f = 1.9=899;
  fp = fopen("data.bin","wb");//data.bin is opened in writing mode as binary
  fwrite(&x,sizeof(int),1,fp);
  fwrite(&f,sizeof(double),1,fp);
  fclose(fp);
}
```

w = write mode 
b = mode as binary
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
- **ptr**: Yazılacak verinin bulunduğu belleğin adresini işaret eden bir işaretçi (pointer).
- **size**: Her bir öğenin bayt cinsinden boyutu.
- **nmemb**: Yazılacak öğe sayısı (how much).
- **stream**: Verinin yazılacağı dosya akışı (where).


## A small program for reading data from the file
data.bin was written above so this is like a continuation
```c
int main()
{
  FILE *fp;
  int x;
  double f;
  fp = fopen("data.bin","rb");
  fread(&x,sizeof(int),1,fp);
  fread(&f,sizeof(double),1,fp);

  printf("x value read from the file:%d",x);
  printf("f value read from the file:%f",f);
  fclose(fp);
}

```

+ fread has same prototype as fwrite
- **Note**: Check how can we write and read files with arrays in C. It is similar to the above but need to check it.

Gecen dersteki kodlari inceledik hangileri onemli onemsiz ve byte'larin kac olduguna baktik


**Palette Size**: Renk paleti boyutu, bibitcount değerine bağlıdır:

- 1-bit: 2 renk × 4 bayt = 8 bayt
- 4-bit: 16 renk × 4 bayt = 64 bayt
- 8-bit: 256 renk × 4 bayt = 1024 bayt
- 24-bit veya daha yüksek: Genellikle palet yoktur (0 bayt).

| bibitCount | Height | Width | Image File Header Size | Image Info Header Size | Palatte Size                                     | Data Size(Bytes)            | Total Size(Bytes) |
| ---------- | ------ | ----- | ---------------------- | ---------------------- | ------------------------------------------------ | --------------------------- | ----------------- |
| 1          | 100    | 51    | 14                     | 40                     | 4*(2^1) =8                                       | 800 = Palatte Size * Height | 862               |
| 4          | 100    | 51    | 14                     | 40                     | 4*(2^4)=64                                       | 2800                        | 2918              |
| 8          | 100    | 51    | 14                     | 40                     | 4*(2^8)=1024                                     | 5200                        | 6278              |
| 24         | 100    | 51    | 14                     | 40                     | 4*(2^24)=0 there is no pallate for 24 bibitCount | 15600                       | 15654             |

Pixel Data Size: Piksel verilerinin boyutu, width, height, ve bibitcount’a bağlıdır. Her satır, 4 bayta hizalanır (padding).

51 * 1 = 51 bits= 6,375 bytes          the line size must be 8 bytes 
51 * 4 = 204 bits= 25,5 bytes        the line size must be 28 bytes
51 * 8 = 408 bits= 51 bytes            the line size must be 52 bytes
51 * 24 = 1224 bits= 153 bytes      the line size must be 156 bytes

So the :
+ **Total Size**  = Data Size + Palatte Size + Image Info Header Size + Image File Header Size


## Aciklamasi (Grok)
# BMP Dosya Boyutu Hesaplama

Bu döküman, farklı `bibitcount` değerleri (piksel başına bit sayısı) için bir BMP dosyasının boyutlarını ve bileşenlerini açıklayan bir tabloyu detaylı bir şekilde ele alıyor. Tablo, sabit yükseklik (`height = 100`), genişlik (`width = 51`), dosya başlığı boyutu (`Image File Header Size = 14 bayt`), bilgi başlığı boyutu (`Image Info Header Size = 40 bayt`), palet boyutu, piksel verisi boyutu (`Data Size`) ve toplam dosya boyutu (`Total Size`) içeriyor. Aşağıda, tablo satır satır açıklanmakta ve her bir değerin nasıl hesaplandığı BMP dosya yapısına uygun şekilde anlatılmaktadır.

## Tablo

|bibitCount|Height|Width|Image File Header Size|Image Info Header Size|Palette Size|Data Size (Bytes)|Total Size (Bytes)|
|---|---|---|---|---|---|---|---|
|1|100|51|14|40|4*(2^1) = 8|800|862|
|4|100|51|14|40|4*(2^4) = 64|2800|2918|
|8|100|51|14|40|4*(2^8) = 1024|5200|6278|
|24|100|51|14|40|0 (no palette)|15600|15654|

## Tablo Açıklaması

### Genel Formül

BMP dosyasının toplam boyutu (`Total Size`) şu şekilde hesaplanır:

```text
Total Size = Image File Header Size + Image Info Header Size + Palette Size + Data Size
```

- **Image File Header Size**: BMP dosya başlığı, sabit 14 bayttır (`BMPFH` yapısı).
- **Image Info Header Size**: BMP bilgi başlığı, genellikle 40 bayttır (Windows BITMAPINFOHEADER için).
- **Palette Size**: `bibitcount`’a bağlı olarak renk paletinin bayt cinsinden boyutu.
- **Data Size**: Piksel verilerinin bayt cinsinden boyutu, `width`, `height` ve `bibitcount`’a bağlıdır.

Piksel verisi boyutu (`Data Size`) şu formülle hesaplanır:

- **Row Size (satır boyutu)**: `(width * bibitcount + 31) / 32 * 4` (4 bayta hizalanır).
- **Data Size**: `Row Size * height`.

### Satır Satır Açıklama

#### 1. Satır: bibitcount = 1

- **bibitCount = 1**: Her piksel 1 bit ile temsil edilir (2 renk: siyah/beyaz).
- **Height = 100, Width = 51**: Görüntü 51 piksel genişlikte ve 100 piksel yükseklikte.
- **Image File Header Size = 14**: BMP dosya başlığı sabit 14 bayt.
- **Image Info Header Size = 40**: BMP bilgi başlığı sabit 40 bayt.
- __Palette Size = 4_(2^1) = 8_*:
    - 1-bit görüntü, 2 renk içerir (2^1 = 2).
    - Her renk 4 bayt (RGB + rezerv bayt): `2 * 4 = 8 bayt`.
- **Data Size = 800 bayt**:
    - Satır boyutu: `(width * bibitcount + 31) / 32 * 4 = (51 * 1 + 31) / 32 * 4 = (51 + 31) / 32 * 4 = 82 / 32 * 4 = 2 * 4 = 8 bayt`.
    - Toplam veri boyutu: `Row Size * height = 8 * 100 = 800 bayt`.
- **Total Size = 862 bayt**:
    - `14 + 40 + 8 + 800 = 862 bayt`.
- **Doğrulama**: Hesaplamalar doğru, toplam boyutu oluşturuyor.

#### 2. Satır: bibitcount = 4

- **bibitCount = 4**: Her piksel 4 bit ile temsil edilir (16 renk: 2^4).
- **Height = 100, Width = 51**: Aynı boyutlar.
- **Image File Header Size = 14**: Sabit 14 bayt.
- **Image Info Header Size = 40**: Sabit 40 bayt.
- __Palette Size = 4_(2^4) = 64_*:
    - 4-bit görüntü, 16 renk içerir (2^4 = 16).
    - Her renk 4 bayt: `16 * 4 = 64 bayt`.
- **Data Size = 2800 bayt**:
    - Satır boyutu: `(width * bibitcount + 31) / 32 * 4 = (51 * 4 + 31) / 32 * 4 = (204 + 31) / 32 * 4 = 235 / 32 * 4 = 7 * 4 = 28 bayt`.
    - Toplam veri boyutu: `Row Size * height = 28 * 100 = 2800 bayt`.
- **Total Size = 2918 bayt**:
    - `14 + 40 + 64 + 2800 = 2918 bayt`.
- **Doğrulama**: Hesaplamalar doğru.

#### 3. Satır: bibitcount = 8

- **bibitCount = 8**: Her piksel 8 bit ile temsil edilir (256 renk: 2^8).
- **Height = 100, Width = 51**: Aynı boyutlar.
- **Image File Header Size = 14**: Sabit 14 bayt.
- **Image Info Header Size = 40**: Sabit 40 bayt.
- __Palette Size = 4_(2^8) = 1024_*:
    - 8-bit görüntü, 256 renk içerir (2^8 = 256).
    - Her renk 4 bayt: `256 * 4 = 1024 bayt`.
- **Data Size = 5200 bayt**:
    - Satır boyutu: `(width * bibitcount + 31) / 32 * 4 = (51 * 8 + 31) / 32 * 4 = (408 + 31) / 32 * 4 = 439 / 32 * 4 = 13 * 4 = 52 bayt`.
    - Toplam veri boyutu: `Row Size * height = 52 * 100 = 5200 bayt`.
- **Total Size = 6278 bayt**:
    - `14 + 40 + 1024 + 5200 = 6278 bayt`.
- **Doğrulama**: Hesaplamalar doğru.

#### 4. Satır: bibitcount = 24

- **bibitCount = 24**: Her piksel 24 bit ile temsil edilir (RGB, her renk 8 bit, paletsiz).
- **Height = 100, Width = 51**: Aynı boyutlar.
- **Image File Header Size = 14**: Sabit 14 bayt.
- **Image Info Header Size = 40**: Sabit 40 bayt.
- **Palette Size = 0**:
    - 24-bit görüntüler palet kullanmaz, çünkü her piksel doğrudan RGB değerlerini içerir.
- **Data Size = 15600 bayt**:
    - Satır boyutu: `(width * bibitcount + 31) / 32 * 4 = (51 * 24 + 31) / 32 * 4 = (1224 + 31) / 32 * 4 = 1255 / 32 * 4 = 39 * 4 = 156 bayt`.
    - Toplam veri boyutu: `Row Size * height = 156 * 100 = 15600 bayt`.
- **Total Size = 15654 bayt**:
    - `14 + 40 + 0 + 15600 = 15654 bayt`.
- **Doğrulama**: Hesaplamalar doğru.

## Genel Değerlendirme

- **Hizalama**: BMP formatında her satır 4 bayta hizalanır, bu yüzden `Row Size` hesaplamasında `(width * bibitcount + 31) / 32 * 4` kullanılır. Bu formül, satırın 4 baytın katı olmasını sağlar.
- **Palet**: `bibitcount <= 8` için palet vardır ve boyutu `4 * (2^bibitcount)` formülüyle hesaplanır. `bibitcount = 24` için palet yoktur.
- **Veri Boyutu**: Piksel verisi, `bibitcount`’ın artmasıyla büyür, çünkü her piksel daha fazla bit kullanır. 24-bit’te veri boyutu en büyüktür, çünkü palet yerine doğrudan RGB verisi saklanır.
