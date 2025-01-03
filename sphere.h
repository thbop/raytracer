#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const point3& center, double radius, shared_ptr<Material> mat) : center(center), radius(std::fmax(0,radius)), mat(mat) {
        auto rvec = vec3( radius, radius, radius );
        bbox = AABB( center - rvec, center + rvec );
    }

    bool hit( const ray& r, interval ray_t, HitRecord& rec ) const override {
        vec3 oc = center - r.o;
        auto a = r.dir.length_squared();
        auto h = dot(r.dir, oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if ( discriminant < 0 ) return false;

        auto sqrtd = std::sqrt(discriminant);

        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root)) return false;
        }

        rec.t      = root;
        rec.p      = r.at(rec.t);
        vec3 outwardNormal = ( rec.p - center ) / radius;
        rec.setFaceNormal( r, outwardNormal );
        getUV( outwardNormal, rec.u, rec.v );
        rec.mat = mat;

        return true;
    }

    AABB boundingBox() const override { return bbox; }

private:
    point3 center;
    double radius;
    shared_ptr<Material> mat;
    AABB bbox;

    static void getUV( const point3& p, double& u, double& v ) {
        auto theta = std::acos(-p.y());
        auto phi   = std::atan2( -p.z(), p.x() ) + pi;
        
        u = phi / (2*pi);
        v = theta / pi;
    }
};

#endif