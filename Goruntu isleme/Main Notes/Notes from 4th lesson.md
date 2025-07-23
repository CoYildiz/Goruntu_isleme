# HISTOGRAM
A histogram is a graph. A graph that shows frequency of anything. But an image histogram, shows frequency of pixels intensity values. In an image histogram, the x axis shows the gray level intensities and the y axis shows the frequency of these intensities..
![[Pasted image 20250722172731.png]] The histogram of the above picture of the Einstein would be something like this
![[Pasted image 20250722172817.png]]
The x axis of the histogram shows the range of pixel values. Since its an 8bit image, that means it has 256 levels of gray or shades of gray in it. Whereas on the y axis, is the count of these intensities.
## How we calculate histogram of an image?

```c
// Verilen IMAGE yapısı üzerinden 8-bit gri görüntünün histogramını hesaplayıp çizme fonksiyonu
void histogram(IMAGE *image)
{
int hist[256] = 0;
int h,w,rowsize,i,j;
w = image->bmpih.biw;
h = image->bmpih.bih;
rowsize = (image->bmpih.bibitcount*w+31)/32*4;
for(i=0;i<h;i++)
{
	for(j=0;j<rowsize;j++) hist[image->data[i*rowsize+j]]++;
}
FILE *fp;
fp=fopen("hist.txt","w");

for(i=0;i<256;i++) fprint(fp,"%d\t%d\n",i,hist[i]);

fclose(fp);
}
```


### How to make an image brighter
Brightness can be simply increased or decresed by simple addition or substraction, to the image matrix.
Consider this black image of 5 rows and 5 columns
If we want to make a black image we use 0 so when you increse that value it being bright.You can add level in this function
```c
void allBlack(IMAGE *image)
{
    int h,w,rowsize,i,j;
    w = image->bmpih.biw;
    h = image->bmpih.bih;
    rowsize = (image->bmpih.bibitcount*w+31)/32*4;

    for(i=0;i<h;i++){
        for(j=0;j<rowsize;j++){
            image->data[i*rowsize+j]=0;
        }
    }
    return;
}
```
