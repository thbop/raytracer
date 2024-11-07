#ifndef CORE_H
#define CORE_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi       = 3.1415926535897932385;

inline double deg2rad( double degrees ) { return degrees * pi / 180.0; }

inline double random_double() { return std::rand() / ( RAND_MAX + 1.0 ); }

inline double random_double( double min, double max ) { return min + (max-min)*random_double(); }

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif