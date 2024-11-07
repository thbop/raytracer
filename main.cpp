#include "core.h"
#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"


color rayColor(const ray& r, const Hittable& world) {
    HitRecord rec;
    if ( world.hit(r, 0, infinity, rec) ) {
        return 0.5 * ( rec.normal + color(1,1,1) );
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

    // World
    HittableList world;

    world.add( make_shared<Sphere>(point3(0,0,-1), 0.5) );
    world.add( make_shared<Sphere>(point3(0,-100.5,-1), 100) );

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

            auto pixelColor = rayColor(r, world);
            writeColor(imageFile, pixelColor);
        }
    }
    imageFile.close();
    std::clog << "\rDone.                 \n";
    return 0;
}