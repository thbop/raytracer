#ifndef BVH_H
#define BVH_H

#include "AABB.h"
#include "hittable.h"
#include "hittableList.h"

#include <algorithm>

class BVHNode : public Hittable {
public:
    BVHNode( HittableList list ) : BVHNode( list.objects, 0, list.objects.size() ) {}

    BVHNode( std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end ) {
        bbox = AABB::empty;
        for ( size_t objIndex = start; objIndex < end; objIndex++ )
            bbox = AABB( bbox, objects[objIndex]->boundingBox() );
        
        int axis = bbox.longestAxis();

        auto comparator = ( axis == 0 ) ? box_x_compare
                        : ( axis == 1 ) ? box_y_compare
                                        : box_z_compare;
        
        size_t objSpan = end - start;

        if ( objSpan == 1 ) {
            left = right = objects[start];
        } else if ( objSpan == 2 ) {
            left  = objects[start];
            right = objects[start+1];
        } else {
            std::sort( std::begin(objects) + start, std::begin(objects) + end, comparator );

            auto mid = start + objSpan/2;
            left  = make_shared<BVHNode>( objects, start, mid );
            right = make_shared<BVHNode>( objects, mid, end );
        }

    }

    bool hit(const ray& r, interval ray_t, HitRecord& rec) const override {
        if (!bbox.hit( r, ray_t )) return false;

        bool hitLeft = left->hit( r, ray_t, rec );
        bool hitRight = right->hit( r, interval(ray_t.min, hitLeft ? rec.t : ray_t.max), rec );

        return hitLeft || hitRight;
    }

    AABB boundingBox() const override { return bbox; }

private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB bbox;

    static bool box_compare( const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axisIndex ) {
        auto aAxisInterval = a->boundingBox().axisInterval(axisIndex);
        auto bAxisInterval = b->boundingBox().axisInterval(axisIndex);
        return aAxisInterval.min < bAxisInterval.min;
    }
    static bool box_x_compare( const shared_ptr<Hittable> a, const shared_ptr<Hittable> b ) { return box_compare( a, b, 0 ); }
    static bool box_y_compare( const shared_ptr<Hittable> a, const shared_ptr<Hittable> b ) { return box_compare( a, b, 1 ); }
    static bool box_z_compare( const shared_ptr<Hittable> a, const shared_ptr<Hittable> b ) { return box_compare( a, b, 2 ); }
};


#endif