#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter( const ray& r_in, const HitRecord& rec, color& attenuation, ray& scattered ) const { return false; }

    virtual color emitted() const { return color(0,0,0); }
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

class Dielectric : public Material {
public:
    Dielectric( double refractionIndex ) : refractionIndex(refractionIndex) {} // , fuzz(fuzz)

    bool scatter( const ray& r_in, const HitRecord& rec, color& attenuation, ray& scattered ) const override {
        attenuation = color(1.0, 1.0, 1.0); // Let all light through? - Yes, absorb nothing
        double ri = rec.frontFace ? ( 1.0 / refractionIndex ) : refractionIndex;

        vec3 normalDir = normalize(r_in.dir);
        double cosTheta = std::fmin( dot(-normalDir, rec.normal), 1.0 );
        double sinTheta = std::sqrt( 1.0 - cosTheta*cosTheta );

        bool cannotRefract = ri * sinTheta > 1.0;
        vec3 dir;

        if ( cannotRefract || reflectance( cosTheta, ri ) > random_double() ) // Must reflect
            dir = reflect(normalDir, rec.normal); // + ( fuzz * random_normal() );
        
        else // Can refract
            dir = refract( normalDir, rec.normal, ri );


        scattered = ray( rec.p, dir );
        return true;
    }

private:
    double refractionIndex; // IOR
    // double fuzz;            // Roughness

    static double reflectance( double cosine, double refractionIndex ) {
        // Schlick's approx
        auto r0 = ( 1 - refractionIndex ) / ( 1 + refractionIndex );
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow( ( 1 - cosine ), 5 );
    }
};

class Emission : public Material {
public:
    Emission(const color& emit) : emit(emit) {}

    color emitted() const { return emit; }

private:
    color emit;
};



#endif