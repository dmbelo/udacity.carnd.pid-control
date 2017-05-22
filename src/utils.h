#ifndef UTILS_H
#define UTILS_H

#include <math.h>


class Utils {

public:

    Utils();

    virtual ~Utils();

    double SmoothSaturation(double x, double max, double k);

};

#endif /* UTILS_H */

