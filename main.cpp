#include "core.h"
#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"


int main() {
    // World
    HittableList world;

    world.add( make_shared<Sphere>(point3(0,0,-1), 0.5) );
    world.add( make_shared<Sphere>(point3(0,-100.5,-1), 100) );

    // Camera
    Camera camera;

    camera.aspect_ratio    = 16.0 / 9.0;
    camera.imageWidth      = 400;
    camera.samplesPerPixel = 100;
    camera.maxBounces      = 50;

    camera.render("out.ppm", world);

    return 0;
}