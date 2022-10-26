#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char *uc_arrayNew_1d(int _size) 
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

unsigned char* differ(unsigned char *background, unsigned char *foreground, int width, int height, int channel) 
{
    unsigned char *diff = uc_arrayNew_1d(height*width);

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            int sum = 0;
            for (int k = 0; k < channel; k++) 
            {
                sum = sum + abs(background[i*width*channel+j*channel+ k] - foreground[i*width*channel+j*channel+k]);
            }
            if((sum/3) < 55) 
            {
                diff[i*width + j] = 0;
            } 
            else 
            {
                diff[i*width + j] =255;
            }
        }
    }
    return diff;
}

unsigned char* combine(unsigned char *diff, unsigned char *weather_forecast, unsigned char *foreground, int width, int height, int channel) 
{
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            if(diff[i*width + j]  == 255) 
            {
                for (int k = 0; k < channel; k++) 
                {
                    weather_forecast[i*width*channel+j*channel+ k] = foreground[i*width*4+j*4+k];
                }
            }
        }
    }
}

int main() 
{
    int width, height, channel;

    char path_background[] = "./images/background.png";
    char path_foreground[] = "./images/foreground.png";

    unsigned char * background = stbi_load(path_background , &width , &height , & channel, 0);
    unsigned char * foreground = stbi_load(path_foreground , &width , &height , & channel, 0);

    if(background == NULL || foreground == NULL) 
    {
        printf ("\nError in loading the image\n");
        exit(1);
    }

    // the differences between two images
    unsigned char *diff =differ(background, foreground, width, height, channel);

    char path_bgforecast[] = "./images/weather_forecast-resized.jpg";
    char save_path[] = "./images/finalimage.png";
    
    unsigned char * weather_forecast = stbi_load(path_bgforecast , &width , &height , &channel, 0);

    if(weather_forecast == NULL) 
    {
        printf ("\nError in loading the image\n");
        exit(1);
    }

    printf ("Width = %d\tHeight = %d\tChannel = %d\n", width , height , channel);

    combine(diff, weather_forecast, foreground, width, height, channel);

    // save image
    stbi_write_png(save_path, width, height, channel, weather_forecast, width*channel);
    printf("Final image saved in %s\n", save_path);
}