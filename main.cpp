#include "core.h"

#include "BVH.h"
#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"


int main() {
    // World
    HittableList world;

    // Textures
    auto texture_checker = make_shared<CheckerTexture>(0.32, color(1,1,1), color(0,0,0));
    
    // Materials
    auto material_blue    = make_shared<Lambertian>(color(0.3, 0.7, 0.7));
    auto material_checker = make_shared<Lambertian>(texture_checker);
    auto material_gold    = make_shared<Metal>     (color(0.64, 0.51, 0.15), 0.1);
    auto material_mirror  = make_shared<Metal>     (color(1.0, 1.0, 1.0), 0.5);
    auto material_light   = make_shared<Emission>  (color(1.0, 1.0, 1.0));
    auto material_glass   = make_shared<Dielectric>(1.50);
    auto material_bubble  = make_shared<Dielectric>(1.00 / 1.50);

    // Objects
    world.add( make_shared<Sphere>(point3(2,0,-1), 0.5, material_mirror) );
    world.add( make_shared<Sphere>(point3(1,0,-1), 0.5, material_gold) );
    world.add( make_shared<Sphere>(point3(0,0,-1), 0.5, material_blue) );

    world.add( make_shared<Sphere>(point3(-1,0,-1), 0.5, material_glass) );
    world.add( make_shared<Sphere>(point3(-1,0,-1), 0.4, material_bubble) );

    world.add( make_shared<Sphere>(point3(0,-1000.5,-1), 1000, material_checker) );

    world = HittableList(make_shared<BVHNode>(world));

    // Camera
    Camera camera;

    camera.aspect_ratio    = 16.0 / 9.0;
    camera.imageWidth      = 400;
    camera.vfov            = 30;
    camera.lookFrom        = point3(-3,1,2);
    camera.lookAt          = point3(0,0,-1);

    camera.samplesPerPixel = 1024;
    camera.maxBounces      = 50;

    camera.defocusAngle    = 3.0;
    camera.focusDist       = (camera.lookAt - camera.lookFrom).length();

    // camera.background      = color(0.03, 0.03, 0.03);

    camera.render("out.ppm", world);

    return 0;
}
