#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>


class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList( shared_ptr<Hittable> object ) { add(object); }

    void add( shared_ptr<Hittable> object ) {
        objects.push_back( object );
    }

    bool hit( const ray& r, interval ray_t, HitRecord& rec ) const override {
        HitRecord tempRec;
        bool hitAnything = false;
        auto closestSoFar = ray_t.max;

        for ( const auto& object : objects ) {
            if ( object->hit( r, interval(ray_t.min, closestSoFar), tempRec ) ) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
};

#endif