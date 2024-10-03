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

    std::vector<double> sumVectorsInDataset(std::vector<Centroid*>& centroid_list) {
        if (centroid_list.empty()) {
            throw std::invalid_argument("Dataset must not be empty.");
        }
        
        size_t data_dimension = centroid_list[0]->getCoordinate().size();
        std::vector<double> result(data_dimension, 0.0);

        for (auto centroid : centroid_list) {
            if (centroid->getCoordinate().size() != data_dimension) {
                throw std::invalid_argument("All vectors in the dataset must have the same dimension.");
            }
            for (size_t j = 0; j < data_dimension; ++j) {
                result[j] += centroid->getCoordinate()[j];
            }
        }
        return result;
    }

    std::vector<double> sumVectorsInDataset(std::vector<Centroid*>& centroid_list, std::vector<int> centroid_id_list) {
        if (centroid_list.empty()) {
            throw std::invalid_argument("Dataset must not be empty.");
        }
        
        size_t data_dimension = centroid_list[0]->getCoordinate().size();
        std::vector<double> result(data_dimension, 0.0);

        for (int centroid_id : centroid_id_list) {
            if (centroid_list[centroid_id]->getCoordinate().size() != data_dimension) {
                throw std::invalid_argument("All vectors in the dataset must have the same dimension.");
            }
            for (size_t j = 0; j < data_dimension; ++j) {
                result[j] += centroid_list[centroid_id]->getCoordinate()[j];
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

    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        const std::vector<std::vector<double>>& dataset, int data_scale) {

        std::vector<int> point_ids(2, 0);       // [farthest point id, second ... id]
        std::vector<double> distances(2, 0.0);  // [farthest distance, second ...]
        for (int point_id = 0; point_id < data_scale; point_id++) {
            double dis = distance1(center, dataset[point_id]);

            if (dis >= distances[0]) {
                point_ids[1] = point_ids[0];
                point_ids[0] = point_id;
                distances[1] = distances[0];
                distances[0] = dis;
            } 
            else if (dis > distances[1]) {
                point_ids[1] = point_id;
                distances[1] = dis;
            }
        }
        return point_ids;
    }

    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        const std::vector<std::vector<double>>& dataset, std::vector<int> point_id_list) {

        std::vector<int> point_ids(2, 0);       // [farthest point id, second ... id]
        std::vector<double> distances(2, 0.0);  // [farthest distance, second ...]
        for (int point_id : point_id_list) {
            double dis = distance1(center, dataset[point_id]);

            if (dis >= distances[0]) {
                point_ids[1] = point_ids[0];
                point_ids[0] = point_id;
                distances[1] = distances[0];
                distances[0] = dis;
            } 
            else if (dis > distances[1]) {
                point_ids[1] = point_id;
                distances[1] = dis;
            }
        }
        return point_ids;
    }

    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        std::vector<Centroid*>& centroid_list, int data_scale) {

        std::vector<int> point_ids(2, 0);       // [farthest point id, second ... id]
        std::vector<double> distances(2, 0.0);  // [farthest distance, second ...]
        for (int point_id = 0; point_id < data_scale; point_id++) {
            double dis = distance1(center, centroid_list[point_id]->getCoordinate());

            if (dis >= distances[0]) {
                point_ids[1] = point_ids[0];
                point_ids[0] = point_id;
                distances[1] = distances[0];
                distances[0] = dis;
            } 
            else if (dis > distances[1]) {
                point_ids[1] = point_id;
                distances[1] = dis;
            }
        }
        return point_ids;
    }

    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        std::vector<Centroid*>& centroid_list, std::vector<int> centroid_id_list) {

        std::vector<int> point_ids(2, 0);       // [farthest point id, second ... id]
        std::vector<double> distances(2, 0.0);  // [farthest distance, second ...]
        for (int point_id : centroid_id_list) {
            double dis = distance1(center, centroid_list[point_id]->getCoordinate());

            if (dis >= distances[0]) {
                point_ids[1] = point_ids[0];
                point_ids[0] = point_id;
                distances[1] = distances[0];
                distances[0] = dis;
            } 
            else if (dis > distances[1]) {
                point_ids[1] = point_id;
                distances[1] = dis;
            }
        }
        return point_ids;
    }

    void ballTree1nn(std::vector<double> point, Node& root, KnnRes& res, 
        const std::vector<std::vector<double>>& dataset) {
        if (root.isLeaf()) {
            for (int id : root.data_id_list) {
                double distance = distance1(point, dataset[id]);
                if (distance >= res.dis)
                    continue;
                
                res.dis = distance;
                res.id = id;
            }
            return;
        }
        // if is not leaf node, check whether the child nodes in the search area
        double left_dis = distance1(point, root.leftChild->pivot);
        if (res.id == -1 || res.dis > left_dis - root.leftChild->radius) {
            ballTree1nn(point, *(root.leftChild), res, dataset);
        }
        double right_dis = distance1(point, root.rightChild->pivot);
        if (res.id == -1 || res.dis > right_dis - root.rightChild->radius) {
            ballTree1nn(point, *(root.rightChild), res, dataset);
        }
    }

    void ballTree1nn(std::vector<double> point, Node& root, KnnRes& res, 
        std::vector<Centroid*>& centroid_list) {
        if (root.isLeaf()) {
            for (int id : root.data_id_list) {
                double distance = distance1(point, centroid_list[id]->getCoordinate());
                if (distance >= res.dis)
                    continue;
                
                res.dis = distance;
                res.id = id;
            }
            return;
        }
        // if is not leaf node, check whether the child nodes in the search area
        double left_dis = distance1(point, root.leftChild->pivot);
        if (res.id == -1 || res.dis > left_dis - root.leftChild->radius) {
            ballTree1nn(point, *(root.leftChild), res, centroid_list);
        }
        double right_dis = distance1(point, root.rightChild->pivot);
        if (res.id == -1 || res.dis > right_dis - root.rightChild->radius) {
            ballTree1nn(point, *(root.rightChild), res, centroid_list);
        }
    }

    void ballTree2nn(std::vector<double> point, Node& root, std::vector<KnnRes*>& res, 
        const std::vector<std::vector<double>>& dataset) {
        if (root.isLeaf()) {
            for (int id : root.data_id_list) {
                double distance = distance1(point, dataset[id]);
                if (res[0]->id == -1) {
                    res[0]->dis = distance;
                    res[0]->id = id;
                } else if (distance < res[0]->dis) {
                    res[1]->id = res[0]->id;
                    res[1]->dis = res[0]->dis;
                    res[0]->id = id;
                    res[0]->dis = distance;
                } else if (distance < res[1]->dis) {
                    res[1]->id = id;
                    res[1]->dis = distance;
                }
            }
            return;
        }
        // if is not leaf node, check whether the child nodes in the search area
        double left_dis = distance1(point, root.leftChild->pivot);
        if (res[1]->id == -1 || res[1]->dis > left_dis - root.leftChild->radius) {
            ballTree2nn(point, *(root.leftChild), res, dataset);
        }
        double right_dis = distance1(point, root.rightChild->pivot);
        if (res[1]->id == -1 || res[1]->dis > right_dis - root.rightChild->radius) {
            ballTree2nn(point, *(root.rightChild), res, dataset);
        }
    }

    void ballTree2nn(std::vector<double> point, Node& root, std::vector<KnnRes*>& res, 
        std::vector<Centroid*>& centroid_list) {
        if (root.isLeaf()) {
            for (int id : root.data_id_list) {
                double distance = distance1(point, centroid_list[id]->getCoordinate());
                if (res[0]->id == -1) {
                    res[0]->dis = distance;
                    res[0]->id = id;
                } else if (distance < res[0]->dis) {
                    res[1]->id = res[0]->id;
                    res[1]->dis = res[0]->dis;
                    res[0]->id = id;
                    res[0]->dis = distance;
                } else if (distance < res[1]->dis) {
                    res[1]->id = id;
                    res[1]->dis = distance;
                }
            }
            return;
        }
        // if is not leaf node, check whether the child nodes in the search area
        double left_dis = distance1(point, root.leftChild->pivot);
        if (res[1]->id == -1 || res[1]->dis > left_dis - root.leftChild->radius) {
            ballTree2nn(point, *(root.leftChild), res, centroid_list);
        }
        double right_dis = distance1(point, root.rightChild->pivot);
        if (res[1]->id == -1 || res[1]->dis > right_dis - root.rightChild->radius) {
            ballTree2nn(point, *(root.rightChild), res, centroid_list);
        }
    }
}