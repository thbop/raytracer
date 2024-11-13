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

    while (window.isOpen()) {
        window.pollEvents(event);

        window.clearScreen();

        window.plotLine( 50, 50, 100, 97, color(1,0,1) );

        window.flip();
    }

    window.destroy();

    return 0;
}