#include "core.h"
#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"


int main() {
    // World
    HittableList world;

    auto material_blue   = make_shared<Lambertian>(color(0.3, 0.7, 0.7));
    auto material_gold   = make_shared<Metal>     (color(0.3, 0.3, 0.1), 0.0);
    auto material_mirror = make_shared<Metal>     (color(1.0, 1.0, 1.0), 0.4);

    world.add( make_shared<Sphere>(point3(0.5,0,-1), 0.5, material_mirror) );
    world.add( make_shared<Sphere>(point3(-0.5,0,-1), 0.5, material_mirror) );
    world.add( make_shared<Sphere>(point3(0,-100.5,-1), 100, material_blue) );

    // Camera
    Camera camera;

    camera.aspect_ratio    = 16.0 / 9.0;
    camera.imageWidth      = 400;
    camera.samplesPerPixel = 100;
    camera.maxBounces      = 50;

    camera.render("out.ppm", world);

    return 0;
}