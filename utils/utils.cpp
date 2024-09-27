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
}
