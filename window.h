#ifndef WINDOW_H
#define WINDOW_H


#include <cstdlib>

#include "SDL2/SDL.h"
#include "vec3.h"

#define W_WINDOW_WIDTH 1600
#define W_WINDOW_HEIGHT 900
#define W_SCREEN_WIDTH 400
#define W_SCREEN_HEIGHT 250

typedef unsigned long u32;
typedef unsigned char u8;

#define W_CHECK_ERROR(test, message) \
    do { \
        if((test)) { \
            fprintf(stderr, "%s\n", (message)); \
            exit(1); \
        } \
    } while(0)

class Window {
public:
    Window( const char* name ) {
        W_CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());
        window = SDL_CreateWindow(
            name,
            SDL_WINDOWPOS_CENTERED_DISPLAY(0),
            SDL_WINDOWPOS_CENTERED_DISPLAY(0),
            W_WINDOW_WIDTH, W_WINDOW_HEIGHT,
            SDL_WINDOW_ALLOW_HIGHDPI
        );
        W_CHECK_ERROR(window == NULL, SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        W_CHECK_ERROR(renderer == NULL, SDL_GetError());

        texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            W_SCREEN_WIDTH, W_SCREEN_HEIGHT
        );
        W_CHECK_ERROR(texture == NULL, SDL_GetError());

        running = true;

    }

    void pollEvents( SDL_Event& event ) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
    }

    void clearScreen() {
        memset(pixels, 0, sizeof(pixels));
    }

    bool isOpen() const { return running; }

    void setPixel(color c, u32 x, u32 y) {
        u32 byteColor = convertColor( c );
        pixels[ y * W_SCREEN_WIDTH + x ] = byteColor;
    }

    void flip() {
        SDL_UpdateTexture(texture, NULL, pixels, W_SCREEN_WIDTH * 4); // 4 = sizeof(u32)
        SDL_RenderCopyEx(
            renderer, texture,
            NULL, NULL, 0.0, NULL, SDL_FLIP_NONE
        );
        SDL_RenderPresent(renderer);
    }

    void destroy() {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

private:
    SDL_Window   *window;
    SDL_Texture  *texture;
    SDL_Renderer *renderer;
    u32          pixels[W_SCREEN_WIDTH * W_SCREEN_HEIGHT];
    bool         running;

    static u8 floatToByte( float value ) {
        if ( value <= 0.0 ) return 0;
        if ( 1.0 <= value ) return 255;
        return static_cast<unsigned char>( 256.0 * value );
    }

    static u32 convertColor( color c ) {
        // RRGGBBAA
        return
            (u32(floatToByte(c.x())) << 24) |
            (u32(floatToByte(c.y())) << 16) |
            (u32(floatToByte(c.z())) << 8 ) | 0xFF;
    }
};

#endif