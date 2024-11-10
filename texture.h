#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
    virtual ~Texture() = default;

    virtual color value( double u, double v, const point3& p ) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor( const color& albedo ) : albedo(albedo) {}
    SolidColor( double red, double green, double blue ) : albedo(color(red, green, blue)) {}

    color value( double u, double v, const point3& p ) const override {
        return albedo;
    }
private:
    color albedo;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture( double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd )
        : invScale( 1.0 / scale ), even(even), odd(odd) {}

    CheckerTexture( double scale, const color& c0, const color& c1 )
        : invScale( 1.0 / scale ), even(make_shared<SolidColor>(c0)), odd(make_shared<SolidColor>(c1)) {}
    
    color value( double u, double v, const point3& p ) const override {
        auto xInteger = int( std::floor( invScale * p.x() ) );
        auto yInteger = int( std::floor( invScale * p.y() ) );
        auto zInteger = int( std::floor( invScale * p.z() ) );

        bool isEven = ( xInteger + yInteger + zInteger ) % 2 == 0;

        return isEven ? even->value( u, v, p ) : odd->value( u, v, p );
    }
private:
    double invScale;
    shared_ptr<Texture> even, odd;
};

#endif