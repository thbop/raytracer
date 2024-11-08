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
    auto material_dark   = make_shared<Lambertian>(color(0.1, 0.1, 0.1));
    auto material_gold   = make_shared<Metal>     (color(0.3, 0.3, 0.1), 0.3);
    auto material_mirror = make_shared<Metal>     (color(1.0, 1.0, 1.0), 0.2);
    auto material_light  = make_shared<Emission>  (color(1.0, 1.0, 1.0));
    auto material_glass  = make_shared<Dielectric>(1.50);
    auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);

    world.add( make_shared<Sphere>(point3(0,0,-1), 0.5, material_blue) );
    world.add( make_shared<Sphere>(point3(1,0,-1), 0.5, material_gold) );

    world.add( make_shared<Sphere>(point3(-1,0,-1), 0.5, material_glass) );
    world.add( make_shared<Sphere>(point3(-1,0,-1), 0.4, material_bubble) );

    world.add( make_shared<Sphere>(point3(0,-1000.5,-1), 1000, material_dark) );

    // Camera
    Camera camera;

    camera.aspect_ratio    = 16.0 / 9.0;
    camera.imageWidth      = 400;
    camera.vfov            = 20;
    camera.lookFrom        = point3(-2,2,1);
    camera.lookAt          = point3(0,0,-1);

    camera.samplesPerPixel = 100;
    camera.maxBounces      = 50;

    camera.defocusAngle    = 10.0;
    camera.focusDist       = 3.4;

    // camera.background      = color(0.03, 0.03, 0.03);

    camera.render("out.ppm", world);

    return 0;
}