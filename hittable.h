#ifndef HIT_H
#define HIT_H

#include "ray.h"
#include "AABB.h"

class Material; // To be defined later

class HitRecord {
public:
    point3 p;
    vec3 normal;
    shared_ptr<Material> mat;
    double t;
    double u, v;
    bool frontFace;

    void setFaceNormal( const ray& r, const vec3& outwardNormal ) {
        // If the ray is inside the object (or hitting from a different side) invert normal
        frontFace = dot( r.dir, outwardNormal ) < 0;
        normal    = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, HitRecord& rec) const = 0;

    virtual AABB boundingBox() const = 0;
};

#endif