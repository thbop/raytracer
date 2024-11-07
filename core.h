#ifndef CORE_H
#define CORE_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi       = 3.1415926535897932385;

inline double deg2rad( double degrees ) { return degrees * pi / 180.0; }

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif