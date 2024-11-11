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

    double vfov            = 90; // Verticle FOV
    point3 lookFrom        = point3(0,0,0);
    point3 lookAt          = point3(0,0,-1);
    vec3   vup             = vec3(0,1,0);

    double defocusAngle    = 0;
    double focusDist      = 10;

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
    vec3   pixelDeltaU, pixelDeltaV;
    vec3   u, v, w;
    vec3   defocusDiskU, defocusDiskV;

    void initialize() {
        imageHeight = (int(imageWidth / aspect_ratio) > 1) ? int(imageWidth / aspect_ratio) : 1;

        pixelSampleScale = 1.0 / samplesPerPixel;

        center = lookFrom;

        // Viewport stuff
        auto theta = deg2rad( vfov );
        auto h = std::tan( theta/2 );
        auto viewportHeight = 2 * h * focusDist;
        auto viewportWidth = viewportHeight * ((double)imageWidth / imageHeight);

        w = normalize( lookFrom - lookAt );
        u = normalize( cross( vup, w ) );
        v = cross(w, u);

        // Screen mappings
        auto viewportU = viewportWidth * u;
        auto viewportV = viewportHeight * -v;

        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        auto viewportUpperLeft = center - (focusDist * w) - viewportU/2 - viewportV/2;
        pixel00Loc = viewportUpperLeft + 0.5 * ( pixelDeltaU + pixelDeltaV );

        // Calc defocus disk
        auto defocusRadius = focusDist * std::tan( deg2rad( defocusAngle / 2 ) );
        defocusDiskU       = u * defocusRadius;
        defocusDiskV       = v * defocusRadius;
    }

    ray getRay( int i, int j ) {
        auto offset = sampleSquare();
        auto pixelSample =
            pixel00Loc                       +
            ((i + offset.x()) * pixelDeltaU) +
            ((j + offset.y()) * pixelDeltaV);
        
        auto rayOrigin = ( defocusAngle <= 0 ) ? center : defocusDiskSample();
        auto rayDir    = pixelSample - rayOrigin;

        return ray( rayOrigin, rayDir );
    }

    vec3 sampleSquare() const {
        return vec3( random_double() -0.5, random_double() - 0.5, 0 );
    }

    point3 defocusDiskSample() const {
        // Random point in the defocus disk
        auto p = random_in_unit_disk();
        return center + ( p[0] * defocusDiskU ) + ( p[1] * defocusDiskV );
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