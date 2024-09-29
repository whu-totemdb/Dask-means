// Node.h
#ifndef NODE_H
#define NODE_H

#include <vector>
#include <../utils/Utils.h>

using namespace Utils;

class Node {
private:
    // basic structure of a tree node
    Node* leftChild;
    Node* rightChild;
    bool isLeaf = false;
    std::vector<int> data_id_list;      // data covered by **leaf node**

    // exclusive to ball tree node
    std::vector<double> pivot;
    double radius;

    // used for dask-means
    std::vector<double> sum_vector;
    int point_number = 0;

public:
    Node() {};

    Node(std::vector<double> pivot, double radius)
        : pivot(pivot), radius(radius) {};

    Node(std::vector<double> pivot, double radius, bool isLeaf)
        : pivot(pivot), radius(radius), isLeaf(isLeaf) {};

    ~Node() {
        delete leftChild;
        delete rightChild;
        leftChild = nullptr;
        rightChild = nullptr;
    };

    void setLeftChild(Node* node) {
        if (node != nullptr) {
            leftChild = node;
        }
    }

    void setRightChild(Node* node) {
        if (node != nullptr) {
            rightChild = node;
        }
    }

    Node* getLeftChild() { return leftChild; }

    Node* getRightChild() { return rightChild; }

    bool isLeaf() { return this->isLeaf; }

    void initLeafNode(std::vector<int> data_id_list, int size) {
        this->isLeaf = true;
        this->data_id_list = data_id_list;
        this->point_number = size;
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