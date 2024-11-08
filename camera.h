#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class Camera {
public:
    double aspect_ratio    = 1.0;
    int    imageWidth      = 100;
    int    samplesPerPixel = 10;
    int    maxBounces      = 10;
    color  background      = color(0,0,0);

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
                    pixelColor += rayColor(r, maxBounces, world);
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

    color rayColor(const ray& r, int depth, const Hittable& world) {
        if (depth <= 0) return color(0,0,0);

        HitRecord rec;
        if ( !world.hit(r, interval(0.001, infinity), rec) ) {
            // return background;
            // Or sky color
            vec3 dirNormal = normalize(r.dir);
            auto a = 0.5*(dirNormal.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }

        ray scattered;
        color attenuation;
        color colorFromEmission = rec.mat->emitted();

        if ( !rec.mat->scatter( r, rec, attenuation, scattered ) ) {
            return colorFromEmission;
        }
        
        color colorFromScatter = attenuation * rayColor( scattered, depth-1, world );

        return colorFromScatter + colorFromEmission;
    }

};

#endif