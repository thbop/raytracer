#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "AABB.h"
#include "hittable.h"

#include <vector>


class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList( shared_ptr<Hittable> object ) { add(object); }

    void add( shared_ptr<Hittable> object ) {
        objects.push_back( object );
        bbox = AABB( bbox, object->boundingBox() );
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

    AABB boundingBox() const override { return bbox; }
private:
    AABB bbox;
};

#endif