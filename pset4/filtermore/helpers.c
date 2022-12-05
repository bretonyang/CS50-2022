#include "helpers.h"
#include <math.h>

void swap(RGBTRIPLE *a, RGBTRIPLE *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            uint8_t avg = (int)round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; ++i)
    {
        if (width % 2 == 0)
        {
            for (int j = 0; j < width / 2; ++j)
            {
                swap(&image[i][j], &image[i][width - 1 - j]);
            }
        }
        else
        {
            for (int j = 0; j < (width - 1) / 2; ++j)
            {
                swap(&image[i][j], &image[i][width - 1 - j]);
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //make a copy of the image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            copy[i][j] = image[i][j];
        }
    }

    //loop through every pixel of the image
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int redsum = 0, greensum = 0, bluesum = 0, count = 0;

            //check for available neighbors and calculate their average. (h for small cube's height and w for its width)
            for (int h = i - 1; h < i + 2; ++h)
            {
                for (int w = j - 1; w < j + 2; ++w)
                {
                    if (h >= 0 && h < height && w >= 0 && w < width)
                    {
                        redsum += copy[h][w].rgbtRed;
                        greensum += copy[h][w].rgbtGreen;
                        bluesum += copy[h][w].rgbtBlue;
                        ++count;
                    }
                }
            }

            //calculate the average and assign it to pixel
            image[i][j].rgbtRed = (int)round((float)redsum / count);
            image[i][j].rgbtGreen = (int)round((float)greensum / count);
            image[i][j].rgbtBlue = (int)round((float)bluesum / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create the GX and GY convolutional kernel
    int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // make a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            copy[i][j] = image[i][j];
        }
    }

    //loop through every pixel
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            // count the sum of x direction and y direction,and the
            //index of the array is in order of RGB (e.g x_sum[1] = x direction sum of Green)
            int x_sum[3] = {0}, y_sum[3] = {0};

            // loop through a 3 x 3 box and calculate the resulting sum
            for (int h = i - 1; h < i + 2; ++h)
            {
                for (int w = j - 1; w < j + 2; ++w)
                {
                    if (h >= 0 && h < height && w >= 0 && w < width)
                    {
                        x_sum[0] += GX[h - i + 1][w - j + 1] * copy[h][w].rgbtRed;
                        x_sum[1] += GX[h - i + 1][w - j + 1] * copy[h][w].rgbtGreen;
                        x_sum[2] += GX[h - i + 1][w - j + 1] * copy[h][w].rgbtBlue;
                        y_sum[0] += GY[h - i + 1][w - j + 1] * copy[h][w].rgbtRed;
                        y_sum[1] += GY[h - i + 1][w - j + 1] * copy[h][w].rgbtGreen;
                        y_sum[2] += GY[h - i + 1][w - j + 1] * copy[h][w].rgbtBlue;
                    }
                }
            }

            // calculate the new value for image[i][j].
            image[i][j].rgbtRed = (uint8_t)fmin(255, round(sqrt(pow((double)x_sum[0], 2.0) + pow((double)y_sum[0], 2.0))));
            image[i][j].rgbtGreen = (uint8_t)fmin(255, round(sqrt(pow((double)x_sum[1], 2.0) + pow((double)y_sum[1], 2.0))));
            image[i][j].rgbtBlue = (uint8_t)fmin(255, round(sqrt(pow((double)x_sum[2], 2.0) + pow((double)y_sum[2], 2.0))));
        }
    }
    return;
}

//fuction for swapping pixels
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp = *a;
    *a = *b;
    *b = temp;
}
