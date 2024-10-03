#include "Node.h"
#include "../utils/Utils.h"

Node::Node() {
    leftChild = nullptr;
    rightChild = nullptr;
}

Node::Node(std::vector<double> pivot, double radius)
    : pivot(pivot), radius(radius) {
    leftChild = nullptr;
    rightChild = nullptr;
}

Node::Node(std::vector<double> pivot, double radius, bool isLeaf)
    : pivot(pivot), radius(radius), leaf(leaf) {
    leftChild = nullptr;
    rightChild = nullptr;
}

Node::~Node() {
    delete leftChild;
    delete rightChild;
    leftChild = nullptr;
    rightChild = nullptr;
}

void Node::initLeafNode(std::vector<int> data_id_list, int size) {
    this->leaf = true;
    this->data_id_list = data_id_list;
    this->point_number = size;
    this->sum_vector = Utils::multiplyVector(pivot, point_number);
    centroid_id_for_data = std::vector(size, -1);
}

// void Node::dataIn(std::vector<double> vec_in) {
//     sum_vector = Utils::addVector(sum_vector, vec_in);
//     point_number += 1;
// }

// void Node::dataOut(std::vector<double> vec_out) {
//     sum_vector = Utils::subtractVector(sum_vector, vec_out);
//     point_number -= 1;
// }

void Node::setAssigned(int id) {
    centroid_id = id;
}