#ifndef AABB_H
#define AABB_H

class AABB {
public:
    interval x, y, z;

    AABB() {}

    AABB( const interval& x, const interval& y, const interval& z ) : x(x), y(y), z(z) {
        padToMinimums();
    }

    AABB( const point3& a, const point3& b ) {
        // Define a box by the two points a and b

        x = ( a[0] <= b[0] ) ? interval( a[0], b[0] ) : interval( b[0], a[0] );
        y = ( a[1] <= b[1] ) ? interval( a[1], b[1] ) : interval( b[1], a[1] );
        z = ( a[2] <= b[2] ) ? interval( a[2], b[2] ) : interval( b[2], a[2] );

        padToMinimums();
    }

    AABB( const AABB& box0, const AABB& box1 ) {
        x = interval( box0.x, box1.x );
        y = interval( box0.y, box1.y );
        z = interval( box0.z, box1.z );
    }

    const interval& axisInterval(int n) const {
        if ( n == 1 ) return y;
        if ( n == 2 ) return z;
        return x;
    }

    bool hit( const ray& r, interval ray_t ) const {
        for ( int axis = 0; axis < 3; axis++ ) {
            const interval& ax = axisInterval(axis);
            const double adInv = 1.0 / r.dir[axis]; // Find the inverse so we don't have to divide twice

            auto t0 = ( ax.min - r.o[axis] ) * adInv;
            auto t1 = ( ax.max - r.o[axis] ) * adInv;

            if ( t0 < t1 ) {
                if ( t0 > ray_t.min ) ray_t.min = t0;
                if ( t1 < ray_t.max ) ray_t.max = t1;
            } else {
                if ( t1 > ray_t.min ) ray_t.min = t1;
                if ( t0 < ray_t.max ) ray_t.max = t0;
            }

            if ( ray_t.max <= ray_t.min ) return false;
        }
        return true;
    }

    int longestAxis() const {

        if (x.size() > y.size()) return x.size() > z.size() ? 0 : 2;
        else                     return y.size() > z.size() ? 1 : 2;
    }

    static const AABB empty, universe;
private:
    void padToMinimums() {

        double delta = 0.0001;
        if ( x.size() < delta ) x = x.expand(delta);
        if ( y.size() < delta ) y = y.expand(delta);
        if ( z.size() < delta ) z = z.expand(delta);
    }
};

const AABB AABB::empty    = AABB(interval::empty, interval::empty, interval::empty);
const AABB AABB::universe = AABB(interval::universe, interval::universe, interval::universe);

#endif