// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cmath>
#include <stdexcept>


namespace Utils {
    // get the distance of two vectors
    double distance1(const std::vector<double>& a, const std::vector<double>& b);

    // get the square of the distance of two vectors
    double distance2(const std::vector<double>& a, const std::vector<double>& b);

    // get the sum vector
    std::vector<double> addVector(const std::vector<double>& a, const std::vector<double>& b);

    // divide a vector by a constant
    std::vector<double> divideVector(const std::vector<double>& v, double c);
}

#endif // UTILS_H
