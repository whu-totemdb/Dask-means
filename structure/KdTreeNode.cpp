#include "KdTreeNode.h"
#include "../utils/Utils.h"
#include <algorithm>

KdTreeNode::KdTreeNode() {
    ub = std::numeric_limits<double>::max();
    lb = std::numeric_limits<double>::max();
    r = 0.0;
}

KdTreeNode::KdTreeNode(int current_dimension, std::vector<double> split_point)
    : current_dimension(current_dimension), split_point(split_point) {
        ub = std::numeric_limits<double>::max();
        lb = std::numeric_limits<double>::max();
        r = 0.0;
    }

KdTreeNode::~KdTreeNode() {
    if (!this->leaf) {
        delete leftChild;
        delete rightChild;
        leftChild = nullptr;
        rightChild = nullptr;
    }
}

// FOR CENTROID INDEX LEAF NODE
void KdTreeNode::initLeafNode(std::vector<int> centroid_id_list) {
    data_id_list = centroid_id_list;
    this->leaf = true;
}

// only called by leaf node (FOR DATA POINT INDEX ONLY)
void KdTreeNode::initLeafNode(const std::vector<std::vector<double>>& dataset, 
        std::vector<int> point_id_list, int size) {
    this->leaf = true;
    this->data_id_list = point_id_list;
    this->point_number = size;
    this->sum_vector = Utils::sumVectorsInDataset(dataset, point_id_list);
    centroid_id_for_data = std::vector(size, -1);

    // set split_point
    current_dimension = Utils::findBestDimension(dataset, point_id_list);
    int axis = current_dimension;

    std::sort(point_id_list.begin(), point_id_list.end(), 
              [&dataset, axis](int a, int b) {
                  return dataset[a][axis] < dataset[b][axis];
              });

    int median_index = size / 2;
    split_point = dataset[point_id_list[median_index]];

    for (int id : point_id_list) {
        r = std::max(r, Utils::distance1(split_point, dataset[id]));
    }
}

void KdTreeNode::resetBound(const std::vector<std::vector<double>>& dataset,
        KdTreeNode& node, std::vector<Centroid*>& centroid_list) {
    // find two nearest centroid use kd-tree 2nn
    std::vector<KnnRes*> res(2);
    for (size_t i = 0; i< 2; i++) {
        res[i] = new KnnRes();
    }
    Utils::kdTree2nn(split_point, node, res, centroid_list);
    // Utils::calculate2nn(split_point, res, centroid_list);

    // set upper bound and lower bound
    ub = getUpperBound(dataset, centroid_list[res[0]->id]->getCoordinate());
    lb = getLowerBound(dataset, centroid_list[res[1]->id]->getCoordinate());
}

double KdTreeNode::getUpperBound(const std::vector<std::vector<double>>& dataset,
        std::vector<double> point) {
    if (!leaf) {
        double ub1 = this->leftChild->getUpperBound(dataset, point);
        double ub2 = this->rightChild->getUpperBound(dataset, point);
        return std::max(ub1, ub2);
    }

    double ub = 0.0;
    for (int id : data_id_list) {
        ub = std::max(ub, Utils::distance1(dataset[id], point));
    }
    return ub;
}

double KdTreeNode::getLowerBound(const std::vector<std::vector<double>>& dataset,
        std::vector<double> point) {
    if (!leaf) {
        double lb1 = this->leftChild->getLowerBound(dataset, point);
        double lb2 = this->rightChild->getLowerBound(dataset, point);
        return std::min(lb1, lb2);
    }

    double lb = std::numeric_limits<double>::max();
    for (int id : data_id_list) {
        lb = std::min(lb, Utils::distance1(dataset[id], point));
    }
    return lb;
}