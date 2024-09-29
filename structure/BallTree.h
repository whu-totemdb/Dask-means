// BallTree.h
#ifndef BALLTREE_H
#define DALLTREE_H

#include <vector>

#include <Node.h>

class BallTree {
private:
    Node* root;
    int capacity;   // leaf node capatity
    
public:
    BallTree(int capacity): capacity(capacity) {};
    ~BallTree() { delete root; };

    void buildBallTree(std::vector<std::vector<double>>& dataset, int data_scale, int data_dimension);

    void buildBallTree1(std::vector<std::vector<double>>& dataset, Node* node, 
            std::vector<int> point_id_list, std::vector<int> farthest_point_id);

    void buildBalancedBallTree(std::vector<std::vector<double>>& dataset, int data_scale, int data_dimension);
};

#endif