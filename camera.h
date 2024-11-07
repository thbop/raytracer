#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera {
public:
    double aspect_ratio    = 1.0;
    int    imageWidth      = 100;
    int    samplesPerPixel = 10;

    void render( const char* filename, const Hittable& world ) {
        initialize();
        std::ofstream imageFile(filename);
        imageFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

        for (int j = 0; j < imageHeight; j++) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            for (int i = 0; i < imageWidth; i++) {
                color pixelColor(0,0,0);
                for ( int sample = 0; sample < samplesPerPixel; sample++ ) {
                    ray r = getRay(i, j);
                    pixelColor += rayColor(r, world);
                }
                writeColor( imageFile, pixelSampleScale * pixelColor );
            }
        }
        imageFile.close();
        std::clog << "\rDone.                 \n";
    }

private:
    int imageHeight;
    double pixelSampleScale;
    point3 center;
    point3 pixel00Loc;
    vec3 pixelDeltaU, pixelDeltaV;

    void initialize() {
        imageHeight = (int(imageWidth / aspect_ratio) > 1) ? int(imageWidth / aspect_ratio) : 1;

        pixelSampleScale = 1.0 / samplesPerPixel;

        center = point3(0,0,0);

        // Viewport stuff
        auto focalLength = 1.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = viewportHeight * ((double)imageWidth / imageHeight);

        // Screen mappings
        auto viewportU = vec3(viewportWidth, 0, 0);
        auto viewportV = vec3(0, -viewportHeight, 0);

        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        auto viewportUpperLeft = center - vec3(0, 0, focalLength) - viewportU/2 - viewportV/2;
        pixel00Loc = viewportUpperLeft + 0.5 * ( pixelDeltaU + pixelDeltaV );
    }

    ray getRay( int i, int j ) {
        auto offset = sampleSquare();
        auto pixelSample =
            pixel00Loc                       +
            ((i + offset.x()) * pixelDeltaU) +
            ((j + offset.y()) * pixelDeltaV);
        
        auto rayOrigin = center;
        auto rayDir    = pixelSample - rayOrigin;

        return ray( rayOrigin, rayDir );
    }

    vec3 sampleSquare() const {
        return vec3( random_double() -0.5, random_double() - 0.5, 0 );
    }

    color rayColor(const ray& r, const Hittable& world) {
        HitRecord rec;
        if ( world.hit(r, interval(0, infinity), rec) ) {
            return 0.5 * ( rec.normal + color(1,1,1) );
        }

        vec3 dirNormal = normalize(r.dir);
        auto a = 0.5*(dirNormal.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};

#endif