#ifndef INTERVAL_H // Another class copied from https://raytracing.github.io/books/RayTracingInOneWeekend.html#anintervalclass
#define INTERVAL_H

class interval {
  public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    interval(double min, double max) : min(min), max(max) {}

    interval( const interval& a, const interval& b ) {
        // Enclose between a and b
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp( double x ) const {
        if ( x < min ) return min;
        if ( x > max ) return max;
        return x;
    }

    interval expand( double delta ) {
        auto padding = delta/2;
        return interval( min - padding, max + padding );
    }

    static const interval empty, universe;
};

const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif