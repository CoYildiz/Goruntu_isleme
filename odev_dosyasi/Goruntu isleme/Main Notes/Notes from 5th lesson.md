## Constrast
Constrast can be defined as the difference between maximum and minimum pixel intensity in an image.



============================================================

![[Pasted image 20250722222014.png]]

After doing this, we will be able to enchhance our constast.
```c
void constrastImage(Image *image)
{
    LONG h,w,rowsize,i,j,k,max,min;
    w = image->bmpih.biw;
    h = image->bmpih.bih;
    rowsize = (image->bmpih.bibitcount*w+31)/32*4;
    maxAndMinIntensity(image,max,min);
    for(i=0;i<h;i++)
        for(j=0;j<rowsize;j++)
            image->data[i*rowsize+j] = (image->data[i*rowsize+j]-min)/(max-min)*255;
}
```


## What is Probability Mass Funcion (PMF) ?
PMF stands for probabiliry mass function. As it name suggest, it gives the probability of each number in the data set or you can say that it basically gives the count or frequency of each element.

### How PMF is calculated
First for a matrix