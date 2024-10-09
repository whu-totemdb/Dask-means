// KdTreeNode.h
#ifndef KDTREENODE_H
#define KDTREENODE_H

#include <vector>
#include "Centroid.h"

class KdTreeNode {
public:
    KdTreeNode* leftChild;
    KdTreeNode* rightChild;
    bool leaf = false;
    std::vector<int> data_id_list;      // data covered by **leaf node**

    int centroid_id = -1;   // internal: centroid id the the node is assigned to
    std::vector<int> centroid_id_for_data;  // leaf: centroid id that each data is assiged to

    // exclusive to kd-tree
    int current_dimension = -1;
    std::vector<double> split_point;
    int point_number = 0;
    std::vector<double> sum_vector;

    // bound for dual-tree, re-calculated if pruning failed
    double ub;      // upper bound to the nearest centroid 
    double lb;      // lower bound to the second nearest centroid

public:
    KdTreeNode();

    KdTreeNode(int current_dimension, std::vector<double> split_point);

    ~KdTreeNode();

    void initLeafNode(std::vector<int> centroid_id_list);
    void initLeafNode(const std::vector<std::vector<double>>& dataset, 
            std::vector<int> point_id_list, int size);

    void resetBound(const std::vector<std::vector<double>>& dataset,
            KdTreeNode& node, std::vector<Centroid*>& centroid_list);

protected:
    double getUpperBound(const std::vector<std::vector<double>>& dataset,
            KdTreeNode& node, std::vector<double> point);

    double getLowerBound(const std::vector<std::vector<double>>& dataset,
            KdTreeNode& node, std::vector<double> point);
};

#endif