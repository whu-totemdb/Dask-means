// Utils.h
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include "../structure/Centroid.h"
#include "../structure/Node.h"
#include "../structure/KnnRes.h"

namespace Utils {
    // get the distance of two vectors
    double distance1(const std::vector<double>& a, const std::vector<double>& b);

    // get the square of the distance of two vectors
    double distance2(const std::vector<double>& a, const std::vector<double>& b);

    // get the sum of all vectors in dataset
    std::vector<double> sumVectorsInDataset(const std::vector<std::vector<double>>& dataset);
    std::vector<double> sumVectorsInDataset(const std::vector<std::vector<double>>& dataset, std::vector<int> point_id_list);
    std::vector<double> sumVectorsInDataset(std::vector<Centroid*>& centroid_list);
    std::vector<double> sumVectorsInDataset(std::vector<Centroid*>& centroid_list, std::vector<int> centroid_id_list);

    // get the sum vector
    std::vector<double> addVector(const std::vector<double>& a, const std::vector<double>& b);

    // get the result of (a - b)
    std::vector<double> subtractVector(const std::vector<double>& a, const std::vector<double>& b);

    // divide a vector by a constant
    std::vector<double> divideVector(const std::vector<double>& v, double c);

    // multiply a vector by a constant
    std::vector<double> multiplyVector(const std::vector<double>& v, double c);

    // find two farthest point to the center in dataset
    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        const std::vector<std::vector<double>>& dataset, int data_scale);
    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        const std::vector<std::vector<double>>& dataset, std::vector<int> point_id_list);
    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        std::vector<Centroid*>& centroid_list, int data_scale);
    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        std::vector<Centroid*>& centroid_list, std::vector<int> centroid_id_list);

    // ball-tree knn
    void ballTree1nn(std::vector<double> point, Node& root, KnnRes& res, 
        const std::vector<std::vector<double>>& dataset);
    void ballTree1nn(std::vector<double> point, Node& root, KnnRes& res, 
        std::vector<Centroid*>& centroid_list);
    void ballTree2nn(std::vector<double> point, Node& root, std::vector<KnnRes*>& res, 
        const std::vector<std::vector<double>>& dataset);
    void ballTree2nn(std::vector<double> point, Node& root, std::vector<KnnRes*>& res, 
        std::vector<Centroid*>& centroid_list);
}

#endif // UTILS_H
