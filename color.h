#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3;

inline double linearToGamma( double linearComponent ) {
    if ( linearComponent > 0 ) return std::sqrt(linearComponent);

    return 0.0;
}

void writeColor( std::ostream& out, const color& pixelColor ) {
    static const interval intensity(0.000, 0.999);
    auto r = int( intensity.clamp( linearToGamma( pixelColor.x() ) ) * 255 );
    auto g = int( intensity.clamp( linearToGamma( pixelColor.y() ) ) * 255 );
    auto b = int( intensity.clamp( linearToGamma( pixelColor.z() ) ) * 255 );

    out << r << ' ' << g << ' ' << b << '\n';
}

#endif