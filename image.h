#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>

#include "vec3.h"
#include "color.h"

#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256

void writeImage() {
    std::ofstream imageFile("out.ppm");
    imageFile << "P3\n" << IMAGE_WIDTH << " " << IMAGE_HEIGHT << "\n255\n";

    for (int j = 0; j < IMAGE_HEIGHT; j++) {
        std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - j) << ' ' << std::flush;
        for (int i = 0; i < IMAGE_WIDTH; i++) {
            auto pixelColor = color((double)i/(IMAGE_WIDTH-1), (double)j/(IMAGE_HEIGHT-1), 0.0);
            writeColor(imageFile, pixelColor);
        }
    }
    imageFile.close();
    std::clog << "\rDone.                 \n";
}

#endif