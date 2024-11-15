#include "core.h"
#include "window.h"

#include "BVH.h"
#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"



int main( int argc, char* argv[] ) {
    Window window = Window( "3D Editor" );
    SDL_Event event;

    Camera camera;
    camera.center = point3(0,0,1);
    camera.aspect_ratio = 16.0 / 9.0;
    camera.imageWidth   = W_SCREEN_WIDTH;

    camera.samplesPerPixel = 1;
    camera.maxBounces      = 30;

    camera.targetSamples   = 10;


    HittableList world;

    auto texture_checker = make_shared<CheckerTexture>(0.32, color(1,1,1), color(0,0,0));

    auto material_glass   = make_shared<Dielectric>(1.5);
    auto material_bubble  = make_shared<Dielectric>(1.00 / 1.50);
    auto material_light   = make_shared<Emission>  (color(10,10,10));
    auto material_checker = make_shared<Lambertian>(texture_checker);
    auto material_mirror  = make_shared<Metal>     (color(1.0, 1.0, 1.0), 0.2);
    auto material_blue    = make_shared<Lambertian>(color(0.3, 0.7, 0.7));

    world.add( make_shared<Sphere>(point3(0,0,-1), 0.5, material_glass) );
    world.add( make_shared<Sphere>(point3(0,0,-1), 0.4, material_bubble) );
    world.add( make_shared<Sphere>(point3(0,0,-1), 0.2, material_light) );

    world.add( make_shared<Sphere>(point3(1,0,-1), 0.5, material_mirror) );
    world.add( make_shared<Sphere>(point3(-1,0,-1), 0.5, material_blue) );
    world.add( make_shared<Sphere>(point3(0,-1000.5,-1), 1000, material_checker) );


    world = HittableList(make_shared<BVHNode>(world));



    while (window.isOpen()) {
        window.pollEvents(event);

        const u8* keystate = SDL_GetKeyboardState(NULL);
        camera.update( keystate );

        // window.clearScreen();

        camera.render(window, &world);

        // window.plotLine( 50, 50, 100, 97, color(1,0,1) );
        // window.plotCircle( 100, 64, 50, color(1,0,0) );

        window.flip();
    }

    window.destroy();

    return 0;
}