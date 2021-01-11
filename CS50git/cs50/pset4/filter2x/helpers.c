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
    // For each pixels, compute pixel bluriness
    // From average rgb of a 3 * 3 grid around the pixel

    RGBTRIPLE(*output)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Init
            const int gridLen = 3;
            int average[sizeof(RGBTRIPLE)] = { 0, 0, 0 };
            int pixels = 0;
            // Bluriness grid
            for (int k = i - 1, l = k + gridLen; k < l; k++)
            {
                for (int m = j - 1, n = m + gridLen; m < n; m++)
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
            for (int k = 0; k < sizeof(RGBTRIPLE); k++)
            {
                average[k] = (int) round((float) average[k] / pixels);
            }
            output[i][j].rgbtBlue = average[0];
            output[i][j].rgbtGreen = average[1];
            output[i][j].rgbtRed = average[2];
            pixels = 0;
        }
    }

    // Output
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = output[i][j].rgbtBlue;
            image[i][j].rgbtGreen = output[i][j].rgbtGreen;
            image[i][j].rgbtRed = output[i][j].rgbtRed;
        }
    }

    // Free memory for blurred image
    free(output);

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // For each pixel
    // Compute Gx to detect horizontal borders, Gy for vertical ones
    // Compute new RGB values through sqrt(Gx² + Gy²), capped at 255
    typedef struct
    {
        int rgbtBlue;
        int rgbtGreen;
        int rgbtRed;
    }
    TEMPG;

    // Init
    const int gridLen = 3;
    const int max = 255;
    RGBTRIPLE(*output)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    TEMPG tempGx[3][3];
    TEMPG tempGy[3][3];
    const int Gx[3][3] =
    {
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 }
    };
    const int Gy[3][3] =
    {
        { -1, -2, -1 },
        { 0, 0, 0 },
        { 1, 2, 1 }
    };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // Init
            double rgbtBlue = 0;
            double rgbtGreen = 0;
            double rgbtRed = 0;

            // Compute Gx and Gy
            for (int k = i - 1, l = k + gridLen, column = 0; k < l; k++, column++)
            {
                for (int m = j - 1, n = m + gridLen, row = 0; m < n; m++, row++)
                {
                    // If image[k][l] is not a pixel
                    if (k < 0 || m < 0 || k >= height || m >= width)
                    {
                        // Set all values to 0
                        tempGx[column][row].rgbtBlue = 0;
                        tempGx[column][row].rgbtGreen = 0;
                        tempGx[column][row].rgbtRed = 0;

                        tempGy[column][row].rgbtBlue = 0;
                        tempGy[column][row].rgbtGreen = 0;
                        tempGy[column][row].rgbtRed = 0;

                        continue;
                    }

                    // Apply Gx and Gy matrices
                    tempGx[column][row].rgbtBlue = Gx[column][row] * image[k][m].rgbtBlue;
                    tempGx[column][row].rgbtGreen = Gx[column][row] * image[k][m].rgbtGreen;
                    tempGx[column][row].rgbtRed = Gx[column][row] * image[k][m].rgbtRed;

                    tempGy[column][row].rgbtBlue = Gy[column][row] * image[k][m].rgbtBlue;
                    tempGy[column][row].rgbtGreen = Gy[column][row] * image[k][m].rgbtGreen;
                    tempGy[column][row].rgbtRed = Gy[column][row] * image[k][m].rgbtRed;

                }
            }

            // Sum up values of rgb from TEMPG grids and reset
            for (int k = 0; k < gridLen; k++)
            {
                for (int l = 0; l < gridLen; l++)
                {
                    if (k == 0 && l == 0)
                    {
                        continue;
                    }

                    tempGx[0][0].rgbtBlue += tempGx[k][l].rgbtBlue;
                    tempGx[0][0].rgbtGreen += tempGx[k][l].rgbtGreen;
                    tempGx[0][0].rgbtRed += tempGx[k][l].rgbtRed;

                    tempGy[0][0].rgbtBlue += tempGy[k][l].rgbtBlue;
                    tempGy[0][0].rgbtGreen += tempGy[k][l].rgbtGreen;
                    tempGy[0][0].rgbtRed += tempGy[k][l].rgbtRed;
                }
            }

            // sqrt(Gx² + Gy²)
            rgbtBlue = round(sqrt(pow(tempGx[0][0].rgbtBlue, 2) + pow(tempGy[0][0].rgbtBlue, 2)));
            rgbtGreen = round(sqrt(pow(tempGx[0][0].rgbtGreen, 2) + pow(tempGy[0][0].rgbtGreen, 2)));
            rgbtRed = round(sqrt(pow(tempGx[0][0].rgbtRed, 2) + pow(tempGy[0][0].rgbtRed, 2)));

            // Cap RGB values at 255
            output[i][j].rgbtBlue = (rgbtBlue > max) ? max : rgbtBlue;
            output[i][j].rgbtGreen = (rgbtGreen > max) ? max : rgbtGreen;
            output[i][j].rgbtRed = (rgbtRed > max) ? max : rgbtRed;
        }
    }

    // Output
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = output[i][j].rgbtBlue;
            image[i][j].rgbtGreen = output[i][j].rgbtGreen;
            image[i][j].rgbtRed = output[i][j].rgbtRed;
        }
    }

    free(output);

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    const int max = 255;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Apply sepia formulas
            int sepiaRed = (int) round(0.393 * (float) image[i][j].rgbtRed
                                       + 0.769 * (float) image[i][j].rgbtGreen
                                       + 0.189 * (float) image[i][j].rgbtBlue);
            int sepiaGreen = (int) round(0.349 * (float) image[i][j].rgbtRed
                                         + 0.686 * (float) image[i][j].rgbtGreen
                                         + 0.168 * (float) image[i][j].rgbtBlue);
            int sepiaBlue = (int)  round(0.272 * (float) image[i][j].rgbtRed
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