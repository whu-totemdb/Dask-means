#include "KdTreeNode.h"
#include "../utils/Utils.h"

KdTreeNode::KdTreeNode() {}

KdTreeNode::KdTreeNode(int current_dimension, double split_threshold)
    : current_dimension(current_dimension), split_threshold(split_threshold) {}

KdTreeNode::~KdTreeNode() {
    if (!this->leaf) {
        delete leftChild;
        delete rightChild;
        leftChild = nullptr;
        rightChild = nullptr;
    }
}

// only called by leaf node
void KdTreeNode::initLeafNode(const std::vector<std::vector<double>>& dataset, 
        std::vector<int> point_id_list, int size) {
    this->leaf = true;
    this->data_id_list = point_id_list;
    this->point_number = size;
    this->sum_vector = Utils::sumVectorsInDataset(dataset, point_id_list);
    centroid_id_for_data = std::vector(size, -1);
}