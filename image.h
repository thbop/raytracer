#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "external/stb_image.h"

#include <cstdlib>
#include <iostream>

class Image {
public:
    Image() {}

    Image( std::string& filename ) {
        if ( load(filename) ) return;

        std::cerr << "ERROR: Could not load image '" << filename << "'.\n";
    }

    ~Image() {
        delete[] bdata;
        STBI_FREE(fdata);
    }

    bool load( std::string& filename ) {
        auto n = bytesPerPixel;
        fdata = stbi_loadf( filename.c_str(), &imgWidth, &imgHeight, &n, bytesPerPixel );
        if ( fdata == nullptr ) return false;

        bytesPerScanline = imgWidth * bytesPerPixel;
        convertToBytes();
        return true;
    }

    int width() const { return ( fdata == nullptr ) ? 0 : imgWidth; }
    int height() const { return ( fdata == nullptr ) ? 0 : imgHeight; }

    const unsigned char* pixelData( int x, int y ) const {
        static unsigned char magenta[] = { 255, 0, 255 };
        if ( bdata == nullptr ) return magenta;

        x = clamp(x, 0, imgWidth);
        y = clamp(y, 0, imgHeight);

        return bdata + y*bytesPerScanline + x*bytesPerPixel;
    }

private:
    const int     bytesPerPixel    = 3;
    float         *fdata           = nullptr;
    unsigned char *bdata           = nullptr;
    int           imgWidth         = 0;
    int           imgHeight        = 0;
    int           bytesPerScanline = 0;

    static int clamp(int x, int low, int high) {
        if ( x < low ) return low;
        if ( x < high ) return x;
        return high-1;
    }

    static unsigned char floatToByte( float value ) {
        if ( value <= 0.0 ) return 0;
        if ( 1.0 <= value ) return 255;
        return static_cast<unsigned char>( 256.0 * value );
    }

    void convertToBytes() {
        int totalBytes = imgWidth * imgHeight * bytesPerPixel;
        bdata = new unsigned char[totalBytes];

        auto *bptr = bdata;
        auto *fptr = fdata;
        for ( auto i = 0; i < totalBytes; i++, fptr++, bptr++ )
            *bptr = floatToByte(*fptr);
    }
};

#endif