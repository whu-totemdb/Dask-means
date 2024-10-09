#include "KdTreeNode.h"
#include "../utils/Utils.h"

KdTreeNode::KdTreeNode() {
    ub = std::numeric_limits<double>::max();
    lb = std::numeric_limits<double>::max();
}

KdTreeNode::KdTreeNode(int current_dimension, std::vector<double> split_point)
    : current_dimension(current_dimension), split_point(split_point) {
        ub = std::numeric_limits<double>::max();
        lb = std::numeric_limits<double>::max();
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
}

// only called by leaf node (FOR DATA POINT INDEX ONLY)
void KdTreeNode::initLeafNode(const std::vector<std::vector<double>>& dataset, 
        std::vector<int> point_id_list, int size) {
    this->leaf = true;
    this->data_id_list = point_id_list;
    this->point_number = size;
    this->sum_vector = Utils::sumVectorsInDataset(dataset, point_id_list);
    centroid_id_for_data = std::vector(size, -1);
}

void KdTreeNode::resetBound(const std::vector<std::vector<double>>& dataset,
        KdTreeNode& node, std::vector<Centroid*>& centroid_list) {
    // find two nearest centroid use kd-tree 2nn
    std::vector<KnnRes*> res(2);
    for (size_t i = 0; i< 2; i++) {
        res[i] = new KnnRes();
    }
    Utils::kdTree2nn(split_point, node, res, centroid_list);

    // set upper bound and lower bound
    ub = getUpperBound(dataset, node, centroid_list[res[0]->id]->getCoordinate());
    lb = getLowerBound(dataset, node, centroid_list[res[1]->id]->getCoordinate());
}

double KdTreeNode::getUpperBound(const std::vector<std::vector<double>>& dataset,
        KdTreeNode& node, std::vector<double> point) {
    if (!node.leaf) {
        double ub1 = getUpperBound(dataset, *node.leftChild, point);
        double ub2 = getUpperBound(dataset, *node.rightChild, point);
        return std::max(ub1, ub2);
    }

    double ub = 0.0;
    for (int id : data_id_list) {
        ub = std::max(ub, Utils::distance1(dataset[id], point));
    }
}

double KdTreeNode::getLowerBound(const std::vector<std::vector<double>>& dataset,
        KdTreeNode& node, std::vector<double> point) {
    if (!node.leaf) {
        double ub1 = getLowerBound(dataset, *node.leftChild, point);
        double ub2 = getLowerBound(dataset, *node.rightChild, point);
        return std::min(ub1, ub2);
    }

    double ub = std::numeric_limits<double>::max();
    for (int id : data_id_list) {
        ub = std::min(ub, Utils::distance1(dataset[id], point));
    }
}