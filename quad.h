#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class Quad : public Hittable {
public:
    Quad( const point3& Q, const vec3& u, const vec3& v, shared_ptr<Material> mat ) : Q(Q), u(u), v(v), mat(mat) {
        auto n = cross( u, v );
        normal = normalize( n );
        D = dot( normal, Q );
        w = n / dot(n,n);

        setBoundingBox();
    }

    virtual void setBoundingBox() {
        auto bboxDiagonal0 = AABB( Q, Q + u + v );
        auto bboxDiagonal1 = AABB( Q + u, Q + v );
        bbox = AABB( bboxDiagonal0, bboxDiagonal1 );
    }

    AABB boundingBox() const override { return bbox; }

    bool hit( const ray& r, interval ray_t, HitRecord& rec ) const override {
        auto denom = dot( normal, r.dir );

        if ( std::fabs(denom) < 1e-8 ) return false;

        auto t = ( D - dot( normal, r.o ) ) / denom;
        if ( !ray_t.contains(t) ) return false;

        auto intersection = r.at(t);
        vec3 planarHitPtVec = intersection - Q;
        auto alpha = dot( w, cross( planarHitPtVec, v ) );
        auto beta = dot( w, cross( u, planarHitPtVec ) );

        if ( !isInterior( alpha, beta, rec ) ) return false;

        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.setFaceNormal( r, normal );
        
        return true;
    }

    virtual bool isInterior( double a, double b, HitRecord& rec ) const {
        interval unitInterval = interval(0,1);

        if ( !unitInterval.contains(a) || !unitInterval.contains(b) )
            return false;
        
        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    point3 Q;
    vec3 u, v;
    vec3 w;
    shared_ptr<Material> mat;
    AABB bbox;
    vec3 normal; // Intersection definition
    double D;
};

#endif