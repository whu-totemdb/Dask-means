// utils.cpp
#include "utils.h"

namespace Utils {
    double distance1(const std::vector<double>& a, const std::vector<double>& b) {
        return std::sqrt(distance2(a, b));
    }

    double distance2(const std::vector<double>& a, const std::vector<double>& b) {
        if (a.size() != b.size()) {
            throw std::invalid_argument("Vectors must be of the same dimension.");
        }
        double sum = 0.0;
        for (size_t i = 0; i < a.size(); ++i) {
            sum += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return sum;
    }

    std::vector<double> addVector(const std::vector<double>& a, const std::vector<double>& b) {
        if (a.size() != b.size()) {
            throw std::invalid_argument("Vectors must be of the same dimension.");
        }
        std::vector<double> result(a.size());
        for (size_t i = 0; i < a.size(); ++i) {
            result[i] = a[i] + b[i];
        }
        return result;
    }

    std::vector<double> divideVector(const std::vector<double>& v, double c) {
        if (c == 0) {
            throw std::invalid_argument("Scaling factor cannot be zero.");
        }
        std::vector<double> result(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            result[i] = v[i] / c;
        }
        return result;
    }
}
