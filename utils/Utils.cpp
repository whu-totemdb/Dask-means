// utils.cpp
#include "Utils.h"

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

    std::vector<double> sumVectorsInDataset(const std::vector<std::vector<double>>& dataset) {
        if (dataset.empty()) {
            throw std::invalid_argument("Dataset must not be empty.");
        }
        
        size_t data_scale = dataset.size();
        size_t data_dimension = dataset[0].size();
        std::vector<double> result(data_dimension, 0.0);

        for (size_t i = 0; i < data_scale; ++i) {
            if (dataset[i].size() != data_dimension) {
                throw std::invalid_argument("All vectors in the dataset must have the same dimension.");
            }
            for (size_t j = 0; j < data_dimension; ++j) {
                result[j] += dataset[i][j];
            }
        }
        return result;
    }

    std::vector<double> sumVectorsInDataset(const std::vector<std::vector<double>>& dataset, std::vector<int> point_id_list) {
        if (dataset.empty()) {
            throw std::invalid_argument("Dataset must not be empty.");
        }
        
        size_t data_scale = dataset.size();
        size_t data_dimension = dataset[0].size();
        std::vector<double> result(data_dimension, 0.0);

        for (size_t i : point_id_list) {
            if (dataset[i].size() != data_dimension) {
                throw std::invalid_argument("All vectors in the dataset must have the same dimension.");
            }
            for (size_t j = 0; j < data_dimension; ++j) {
                result[j] += dataset[i][j];
            }
        }
        return result;
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

    std::vector<double> subtractVector(const std::vector<double>& a, const std::vector<double>& b) {
        if (a.size() != b.size()) {
            throw std::invalid_argument("Vectors must be of the same dimension.");
        }
        std::vector<double> result(a.size());
        for (size_t i = 0; i < a.size(); ++i) {
            result[i] = a[i] - b[i];
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

    std::vector<double> multiplyVector(const std::vector<double>& v, double c) {
        std::vector<double> result(v.size());
        for (size_t i = 0; i < v.size(); ++i) {
            result[i] = v[i] * c;
        }
        return result;
    }
}
