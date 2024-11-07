#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {};

    bool hit( const ray& r, double raytmin, double raytmax, HitRecord& rec ) const override {
        vec3 oc = center - r.o;
        auto a = r.dir.length_squared();
        auto h = dot(r.dir, oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if ( discriminant < 0 ) return false;

        auto sqrtd = std::sqrt(discriminant);

        auto root = (h - sqrtd) / a;
        if ( root <= raytmin || raytmax <= root ) {
            root = (h + sqrtd) / a;
            if ( root <= raytmin || raytmax <= root ) return false;
        }

        rec.t      = root;
        rec.p      = r.at(rec.t);
        vec3 outwardNormal = ( rec.p - center ) / radius;
        rec.setFaceNormal( r, outwardNormal );

        return true;
        
    }

private:
    point3 center;
    double radius;
};

#endif