#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include "KdTreeNode.h"

class KdTree {
public:
    KdTreeNode* root;
    int data_dimension; // k value of kd-tree
    int capacity = 1;

public:
    KdTree(int dimensions);
    ~KdTree();

    void buildKdTree(std::vector<std::vector<double>>& dataset, int data_scale);
    void buildKdTree1(KdTreeNode& node, std::vector<std::vector<double>>& dataset, 
                     std::vector<int>& point_id_list);

private:
    int KdTree::findBestDimension(const std::vector<std::vector<double>>& dataset, 
            const std::vector<int>& point_id_list);
};

#endif
