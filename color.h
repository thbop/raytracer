#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using color = vec3;

void writeColor( std::ostream& out, const color& pixelColor ) {
    auto r = (int)(pixelColor.x() * 255.999);
    auto g = (int)(pixelColor.y() * 255.999);
    auto b = (int)(pixelColor.z() * 255.999);

    out << r << ' ' << g << ' ' << b << '\n';
}

#endif