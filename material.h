#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter( const ray& r_in, const HitRecord& rec, color& attenuation, ray& scattered ) const { return false; }
};

class Lambertian : public Material {
public:
    Lambertian( const color& albedo ) : albedo(albedo) {}

    bool scatter( const ray& r_in, const HitRecord& rec, color& attenuation, ray& scattered ) const override {
        auto scatterDir = rec.normal + random_normal();
        if ( scatterDir.near_zero() ) scatterDir = rec.normal;
        scattered = ray(rec.p, scatterDir);
        attenuation = albedo;
        return true;
    }
private:
    color albedo;
};

class Metal : public Material {
public:
    Metal( const color& albedo, double fuzz ) : albedo(albedo), fuzz( fuzz < 1 ? fuzz : 1 ) {}

    bool scatter( const ray& r_in, const HitRecord& rec, color& attenuation, ray& scattered ) const override {
        vec3 reflected = reflect( r_in.dir, rec.normal );
        reflected = normalize(reflected) + ( fuzz * random_normal() );
        scattered = ray( rec.p, reflected );
        attenuation = albedo;
        return ( dot( scattered.dir, rec.normal ) > 0 ); // Make sure fuzz didn't send it into the sphere
    }

private:
    color albedo;
    double fuzz; // Roughness
};

#endif