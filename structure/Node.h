// Node.h
#ifndef NODE_H
#define NODE_H

#include <vector>
#include <../utils/Utils.h>

using namespace Utils;

class Node {
public:
    // basic structure of a tree node
    Node* leftChild;
    Node* rightChild;
    bool leaf = false;
    std::vector<int> data_id_list;      // data covered by **leaf node**

    // exclusive to ball tree node
    std::vector<int> farthest_point_id;
    std::vector<double> pivot;
    double radius;

    // used for dask-means
    std::vector<double> sum_vector;
    int point_number = 0;

public:
    Node() {
        leftChild = nullptr;
        rightChild = nullptr;
    };

    Node(std::vector<double> pivot, double radius)
        : pivot(pivot), radius(radius) {
        leftChild = nullptr;
        rightChild = nullptr;
    };

    Node(std::vector<double> pivot, double radius, bool isLeaf)
        : pivot(pivot), radius(radius), leaf(leaf) {
        leftChild = nullptr;
        rightChild = nullptr;
    };

    ~Node() {
        delete leftChild;
        delete rightChild;
        leftChild = nullptr;
        rightChild = nullptr;
    };

    bool isLeaf() { return this->leaf; }

    void initLeafNode(std::vector<int> data_id_list, int size) {
        this->leaf = true;
        this->data_id_list = data_id_list;
        this->point_number = size;
        this->sum_vector = multiplyVector(pivot, point_number);
    }

    void dataIn(std::vector<double> vec_in) {
        sum_vector = addVector(sum_vector, vec_in);
        point_number += 1;
    }

    void dataOut(std::vector<double> vec_out) {
        sum_vector = subtractVector(sum_vector, vec_out);
        point_number -= 1;
    }
    
};

#endif // NODE_H