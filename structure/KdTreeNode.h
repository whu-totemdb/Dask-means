// KdTreeNode.h
#ifndef KDTREENODE_H
#define KDTREENODE_H

#include <vector>

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
    double split_threshold;
    int point_number = 0;
    std::vector<double> sum_vector;

public:
    KdTreeNode();

    KdTreeNode(int current_dimension, double split_threshold);

    ~KdTreeNode();

    void initLeafNode(const std::vector<std::vector<double>>& dataset, 
            std::vector<int> point_id_list, int size);
};

#endif