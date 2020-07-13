// ASCIIArt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

int cols = 80;

//get average lumninance from img segment
int GetAverageLuminance(unsigned char* data, int width, int x1, int x2, int y1, int y2) {

    int sumL = 0;
    int count = (x2 - x1) * (y2 - y1);
    for (int i = y1; i < y2; i++) {
        for (int j = x1; j < x2; j++) {
            sumL += data[3*(i * width + j)];
        }
    }
    return (sumL / count)+1;
}

int main()
{
    /*
        grayscale sequence values
        source: http://paulbourke.net/dataformats/asciiart/
    */
    const char* gscale1 = R"($@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'. )";
    const char* gscale2 = R"(@%#*+=-:. )";

    char* filepath = (char*)malloc(64);
    std::cout << "File path: " << std::endl;
    std::cin >> filepath;
    std::cout << "nr of columns (pref 80): " << std::endl;
    std::cin >> cols;

    int width, height, comps;
    unsigned char* data = stbi_load(filepath, &width, &height, &comps, 3);
    float scale = (float)width / height;

    float tileWidth = width / cols;
    float tileHeight = (tileWidth / scale)*3.0f;

    int rows = (int)(height / tileHeight);

    //convert image to grayscale
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char luminance = 0.2126 * data[3 * (i * width + j)] +
                0.7152 * data[3 * (i * width + j) + 1] +
                0.0722 * data[3 * (i * width + j) + 2];

            data[3 * (i * width + j)] = luminance;
            data[3 * (i * width + j) + 1] = luminance;
            data[3 * (i * width + j) + 2] = luminance;
        }
    }

    std::cout << cols << " " << rows << std::endl;
    for (int i = 0; i < rows; i++) {

        int y1 = i * tileHeight;
        int y2 = (i + 1) * tileHeight;

        if (i == rows - 1)
            y2 = height;

        for (int j = 0; j < cols; j++) {
            int x1 = j * tileWidth;
            int x2 = (j + 1) * tileWidth;

            if (j == cols - 1)
                j = width;

            int avg = GetAverageLuminance(data, width, x1, x2, y1, y2);

            std::cout << gscale1[69*avg/255];
        }
        std::cout << "\n";
    }
}