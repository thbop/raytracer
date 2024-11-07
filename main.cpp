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

    camera.imageWidth = 400;
    camera.aspect_ratio = 16.0 / 9.0;

    camera.render("out.ppm", world);

    return 0;
}