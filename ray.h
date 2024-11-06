#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    point3 o;
    vec3 dir;

    ray() {}

    ray(const point3& origin, const vec3& direction) {
        o = origin;
        dir = direction;
    }

    point3 at(double t) const {
        return o + t*dir;
    }


};

#endif