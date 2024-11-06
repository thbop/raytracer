#include <iostream>
#include <fstream>

#include "vec3.h"
#include "color.h"
#include "ray.h"

double hitSphere( const point3& center, double radius, const ray& r ) {
    vec3 oc = center - r.o;
    auto a = dot(r.dir, r.dir);
    auto b = -2.0 * dot(r.dir, oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    
    if ( discriminant < 0 ) return -1.0;
    else                    return (-b - std::sqrt(discriminant)) / (2.0*a); // Solve for t
    
}

color rayColor(const ray& r) {
    auto t = hitSphere( point3(0, 0, -1), 0.5, r );
    if ( t > 0.0 ) {
        vec3 N = normalize(r.at(t) - vec3(0, 0, -1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    vec3 dirNormal = normalize(r.dir);
    auto a = 0.5*(dirNormal.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {
    // Image stuff
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr int imageWidth = 400;
    constexpr int imageHeight = (int(imageWidth / aspect_ratio) > 1) ? int(imageWidth / aspect_ratio) : 1;

    std::ofstream imageFile("out.ppm");
    imageFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    // Camera stuff
    constexpr auto focalLength = 1.0;
    constexpr auto viewportHeight = 2.0;
    constexpr auto viewportWidth = viewportHeight * ((double)imageWidth / imageHeight);
    auto cameraCenter = point3(0, 0, 0);

    // Screen mappings
    auto viewportU = vec3(viewportWidth, 0, 0);
    auto viewportV = vec3(0, -viewportHeight, 0);

    auto pixelDeltaU = viewportU / imageWidth;
    auto pixelDeltaV = viewportV / imageHeight;

    auto viewportUpperLeft = cameraCenter - vec3(0, 0, focalLength) - viewportU/2 - viewportV/2;
    auto pixel00Loc = viewportUpperLeft + 0.5 * ( pixelDeltaU + pixelDeltaV );


    for (int j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            auto pixelCenter = pixel00Loc + ( i * pixelDeltaU ) + ( j * pixelDeltaV );
            auto rayDir = pixelCenter - cameraCenter;
            ray r( cameraCenter, rayDir );

            auto pixelColor = rayColor(r);
            writeColor(imageFile, pixelColor);
        }
    }
    imageFile.close();
    std::clog << "\rDone.                 \n";
    return 0;
}