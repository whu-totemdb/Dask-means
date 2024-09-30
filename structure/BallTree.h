// BallTree.h
#ifndef BALLTREE_H
#define BALLTREE_H

#include <vector>

#include <Node.h>

class BallTree {
private:
    Node* root;
    int capacity = 1;   // leaf node capatity
    
public:
    BallTree() { root = new Node(); }
    BallTree(int capacity): capacity(capacity) { root = new Node(); }
    ~BallTree() { delete root; }

    void buildBallTree(std::vector<std::vector<double>>& dataset, int data_scale);

    void buildBallTree1(std::vector<std::vector<double>>& dataset, 
        Node& node, std::vector<int> point_id_list);

    void buildBalancedBallTree(std::vector<std::vector<double>>& dataset, int data_scale, int data_dimension);

    /*function*/
    void createNode(std::vector<std::vector<double>>& dataset,
        int data_scale, Node& node);

    void createNode(std::vector<std::vector<double>>& dataset, 
        std::vector<int> point_id_list, int size, Node& node);

    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        const std::vector<std::vector<double>>& dataset, int data_scale);
    
    std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
        const std::vector<std::vector<double>>& dataset, std::vector<int> point_id_list);

};

#endif