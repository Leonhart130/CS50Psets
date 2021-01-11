#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = (int) round(((float) image[i][j].rgbtBlue
                                       + (float) image[i][j].rgbtGreen
                                       + (float) image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int max = 255;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Apply sepia formulas
            int  sepiaRed = (int) round(0.393 * (float) image[i][j].rgbtRed
                                        + 0.769 * (float) image[i][j].rgbtGreen
                                        + 0.189 * (float) image[i][j].rgbtBlue);
            int sepiaGreen = (int) round(0.349 * (float) image[i][j].rgbtRed
                                         + 0.686 * (float) image[i][j].rgbtGreen
                                         + 0.168 * (float) image[i][j].rgbtBlue);
            int sepiaBlue = (int) round(0.272 * (float) image[i][j].rgbtRed
                                        + 0.534 * (float) image[i][j].rgbtGreen
                                        + 0.131 * (float) image[i][j].rgbtBlue);

            // If result is higher than a byte, replace by 255
            image[i][j].rgbtRed = (sepiaRed > max) ? max : sepiaRed;
            image[i][j].rgbtGreen = (sepiaGreen > max) ? max : sepiaGreen;
            image[i][j].rgbtBlue = (sepiaBlue > max) ? max : sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, rWidth = width - 1; j < rWidth; j++, rWidth--)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][rWidth];
            image[i][rWidth] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // temp to swap original pixels with blurred pixels at the end
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    // For each pixels, compute pixel bluriness
    // From average rgb of a 3 * 3 grid around the pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Init
            int tc = i - 1;
            int tr = j - 1;
            int gridSize = 3;
            int average[sizeof(RGBTRIPLE)] = { 0, 0, 0 };
            int pixels = 0;
            // Bluriness grid
            for (int k = tc, l = tc + gridSize; k < l; k++)
            {
                for (int m = tr, n = tr + gridSize; m < n; m++)
                {
                    // If image[k][l] is not a pixel
                    if (k < 0 || m < 0 || k >= height || m >= width)
                    {
                        continue;
                    }

                    // Increment rgb array and count pixel
                    average[0] += image[k][m].rgbtBlue;
                    average[1] += image[k][m].rgbtGreen;
                    average[2] += image[k][m].rgbtRed;
                    pixels += 1;
                }
            }
            // Store average amounts of rgb
            for (int o = 0; o < sizeof(RGBTRIPLE); o++)
            {
                average[o] = (int) round((float) average[o] / pixels);
            }
            temp[i][j].rgbtBlue = average[0];
            temp[i][j].rgbtGreen = average[1];
            temp[i][j].rgbtRed = average[2];
            pixels = 0;
        }
    }

    // Write temp in image
    for (int p = 0; p < height; p++)
    {
        for (int q = 0; q < width; q++)
        {
            // Can be improved with memcpy
            image[p][q].rgbtBlue = temp[p][q].rgbtBlue;
            image[p][q].rgbtGreen = temp[p][q].rgbtGreen;
            image[p][q].rgbtRed = temp[p][q].rgbtRed;
        }
    }

    // Free memory for blurred image
    free(temp);

    return;
}
