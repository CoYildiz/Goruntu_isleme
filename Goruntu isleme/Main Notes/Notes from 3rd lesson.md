Change Palette

```c
void changePalette(IMAGE *image){
	int i;
	if(image->bmpih.bibitcount==8){
		for(i=0;i<256;i++){
			image->palet[i].rgbblue=i;
			image->palet[i].rgbgreeen=i;
			image->palet[i].rgbred=i;
			image->palet[i].rgbreserved=0;
		}
	}
}
```
This is the original one ,We can change the i value inside the for loops for what we want.


### Writing the image array values in matrix form
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateRandom(int array[], int n)
{
    srand(time(NULL));
    int i;
    for(i=0;i<n;i++)
    {
        array[i]=rand()%100;
    }
    return;
}

int main()
{
    int array[120] ,n=120 ,i,j,w=10,h=12;
    generateRanndom(array,n);
    for(i=0;i<n;i++) printf("%4d",array[i]);
    //How I can print all the values in matrix form
    printf("\n\n\n\n");

    for(i=0;i<h;i++){
        for(j=0;j<w;j++){
            printf("%4d",array[i*w+j]);
        }
        printf("\n");
    }
    return 0;
}
```

### Görüntünün piksel verilerini tersine çevirerek negatif görüntü oluşturan fonksiyon
```c
// Görüntünün piksel verilerini tersine çevirerek negatif görüntü oluşturan fonksiyon

void negativeImage(IMAGE* image) {
// Görüntü boyutları ve satır boyutu
int h = image->bmpih.bih;
int w = image->bmpih.biw;
int rowsize = (w * image->bmpih.bibitcount + 31) / 32 * 4;
// Piksel verilerini tersine çevir (negatif görüntü)
for (int i = 0; i < h; i++) {
	for (int j = 0; j < rowsize; j++) {
		image->data[i * rowsize + j] = 255 - image->data[i * rowsize + j];
		}
	}
printf("Negatif görüntü başarıyla oluşturuldu.\n");
}
```
![[Pasted image 20250722164727.png]]
After negative image
![[Pasted image 20250722164800.png]]
After negative image 
![[Pasted image 20250722164727.png]]
we back to original one.
## Negative Image
$g(x,y) = Tf(x,y)$ where $f(x,y)$ is the imput image, $g(x,y)$ is the processed image.
![[Pasted image 20250722165512.png]]

Negative images are useful for enhancing white or grey detail embedded in dark tedions of an image
	-Note how much clearer the tissue is in the negative image of the mommogram below
	![[Pasted image 20250722165708.png]]
$$ s = \text{intensity}_{\text{max}} - r $$
+ **$$ \text{intensity}_{\text{max}} $$: biggest value of the pixel in the image (for 8 bits 2^8=256, so 255 is the maks value)
- **r**: Giriş piksel yoğunluğu (orijinal görüntünün piksel değeri).

```c
void thresholdImage(IMAGE* image,int threshold) 
{

// Görüntü boyutları ve satır boyutu
int h = image->bmpih.bih;
int w = image->bmpih.biw;
int rowsize = (w * image->bmpih.bibitcount + 31) / 32 * 4;
for (int i = 0; i < h; i++) 
	{
	for (int j = 0; j < rowsize; j++) 
		{
		if(image->data[i*rowsize+j]>threshold) image->data[i* rowsize+j]=255;
		else image->data[i * rowsize + j] =0;
		}
	}
}
```
#### Thresholding Value Nedir?
Thresholding (eşikleme), bir görüntünün piksel değerlerini bir eşik değeriyle karşılaştırarak pikselleri iki veya daha fazla kategoriye ayırma işlemidir. En yaygın kullanım, gri tonlamalı (8-bit) veya tek renk tonlu bir görüntüyü ikili bir görüntüye dönüştürmektir:

- **Eşikleme Değeri (Threshold Value)**: Piksel değerlerinin karşılaştırıldığı sabit bir değerdir (örneğin, 128). Piksel değeri bu eşikten büyükse bir değere (ör. 255, beyaz), küçükse başka bir değere (ör. 0, siyah) atanır.
- **Örnek**:
    - 8-bit bir görüntüde piksel değerleri 0-255 arasındadır.
    - Eşik değeri 128 seçilirse:
        - Piksel değeri ≥ 128 → r =255 (beyaz).
        - Piksel değeri < 128 → r = 0 (siyah).

+ Eger main() fonksiyonunda thresholdImage(image,50); yaparsak:
![[Pasted image 20250722164727.png]]
+ Beyaz kisimlar artacaktir cunku pixel degeri>50 icin r= 255 yani beyaz ayarladik.
![[Pasted image 20250722171946.png]]
+ threshold = 160 icin yukaridaki mantikla siyah kisimlar artacaktir
![[Pasted image 20250722172132.png]]
