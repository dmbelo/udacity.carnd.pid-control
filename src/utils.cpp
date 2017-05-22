#include "utils.h"

template <typename T> int sign(T val) {

    return (T(0) < val) - (val < T(0));

}

Utils::Utils() {}

Utils::~Utils() {} 

double Utils::SmoothSaturation(double x, double max, double k) {

    int s = sign<double> (x);

    return 0.5 * (x + s * max - s * sqrt(pow(x - s * max, 2) + pow(k, 2)));

}