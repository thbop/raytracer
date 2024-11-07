#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3;

void writeColor( std::ostream& out, const color& pixelColor ) {
    static const interval intensity(0.000, 0.999);
    auto r = int( intensity.clamp( pixelColor.x() ) * 255 );
    auto g = int( intensity.clamp( pixelColor.y() ) * 255 );
    auto b = int( intensity.clamp( pixelColor.z() ) * 255 );

    out << r << ' ' << g << ' ' << b << '\n';
}

#endif