#ifndef WINDOW_H
#define WINDOW_H


#include <cstdlib>

#include "SDL2/SDL.h"
#include "vec3.h"

#define W_WINDOW_WIDTH 1600
#define W_WINDOW_HEIGHT 900
#define W_SCREEN_WIDTH 400
#define W_SCREEN_HEIGHT 225

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

    void setPixel(u32 x, u32 y, color c) {
        u32 byteColor = colorTou32( c );
        if ( onScreen(x, y) )
            pixels[ y * W_SCREEN_WIDTH + x ] = byteColor;
    }

    // void avgPixel( u32 x, u32 y, color c ) {
    //     color current = u32ToColor( pixels[ y * W_SCREEN_WIDTH + x ] );
    //     color avg = ( current + c ) * 0.5;
    //     u32 byteColor = colorTou32( avg );

    //     if ( onScreen(x, y) )
    //         pixels[ y * W_SCREEN_WIDTH + x ] = byteColor;
    // }

    void wtAvgPixel( u32 x, u32 y, color c, double newWt, double oldWt ) {
        // oldWt + newWt = 1
        color current = u32ToColor( pixels[ y * W_SCREEN_WIDTH + x ] );
        color avg = current * oldWt + c*newWt;
        setPixel( x, y, avg );
    }

    void plotLine( int x0, int y0, int x1, int y1, color c ) {
        if ( x0 == x1 ) {
            if ( y1 > y0 ) for ( int j = y0; j < y1; j++ ) setPixel( x0, j, c );
            else           for ( int j = y1; j < y0; j++ ) setPixel( x0, j, c );
            return;
        }
        if ( y0 == y1 ) {
            if ( x1 > x0 ) for ( int i = x0; i < x1; i++ ) setPixel( i, y0, c );
            else           for ( int i = x1; i < x0; i++ ) setPixel( i, y0, c );
            return;
        }
        
        if ( std::abs(y1 - y0) < std::abs(x1 - x0) ) {
            if ( x0 > x1 ) plotLineLow( x1, y1, x0, y0, c );
            else           plotLineLow( x0, y0, x1, y1, c );
        }
        else {
            if ( y0 > y1 ) plotLineHigh( x1, y1, x0, y0, c );
            else           plotLineHigh( x0, y0, x1, y1, c );
        }
    }

    void plotCircle( int x, int y, int radius, color c ) {
        int i = radius;
        int j = 0;
        int p = 1 - radius;

        while ( i >= j ) {
            plot8( x, y, i, j, c );

            j++;
            if ( p < 0 ) p += 2 * j + 1;
            else         { i--; p += 2 * (j - i) + 1; }
        }
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

    static u32 colorTou32( color c ) {
        // RRGGBBAA
        return
            (u32(floatToByte(c.x())) << 24) |
            (u32(floatToByte(c.y())) << 16) |
            (u32(floatToByte(c.z())) << 8 ) | 0xFF;
    }

    static color u32ToColor( u32 a ) {
        double inv255 = 0.003921568627;
        return color(
            ((a >> 24) & 0xFF) * inv255,
            ((a >> 16) & 0xFF) * inv255,
            ((a >>  8) & 0xFF) * inv255
        );
    }

    static bool onScreen(u32 x, u32 y) {
        return interval(0, W_SCREEN_WIDTH-1).contains(x) && interval(0, W_SCREEN_HEIGHT-1).contains(y);
    }

    void plotLineLow( int x0, int y0, int x1, int y1, color c ) {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int yi = 1;
        if ( dy < 0 ) {
            yi = -1;
            dy = -dy;
        }
        int D = (2 * dy) - dx;
        int y = y0;

        for ( int x = x0; x < x1; x++ ) {
            setPixel(x, y, c);
            if (D > 0) {
                y += yi;
                D += ( 2 * ( dy - dx ) );
            }
            else
                D += 2 * dy;
        }
    }
    void plotLineHigh( int x0, int y0, int x1, int y1, color c ) {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int xi = 1;
        if ( dx < 0 ) {
            xi = -1;
            dx = -dx;
        }
        int D = (2 * dx) - dy;
        int x = x0;

        for ( int y = y0; y < y1; y++ ) {
            setPixel(x, y, c);
            if (D > 0) {
                x += xi;
                D += ( 2 * ( dx - dy ) );
            }
            else
                D += 2 * dx;
        }
    }

    void plot8( int x, int y, int i, int j, color c ) {
        setPixel( x+i, y+j, c ); setPixel( x-i, y+j, c );
        setPixel( x+i, y-j, c ); setPixel( x-i, y-j, c );
        setPixel( x+j, y+i, c ); setPixel( x-j, y+i, c );
        setPixel( x+j, y-i, c ); setPixel( x-j, y-i, c );
    }
};

#endif