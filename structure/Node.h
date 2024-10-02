// Node.h
#ifndef NODE_H
#define NODE_H

#include <vector>

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
    Node();

    Node(std::vector<double> pivot, double radius);

    Node(std::vector<double> pivot, double radius, bool isLeaf);

    ~Node();

    bool isLeaf() { return this->leaf; }

    void initLeafNode(std::vector<int> data_id_list, int size);

    void dataIn(std::vector<double> vec_in); 

    void dataOut(std::vector<double> vec_out);
    
};

#endif // NODE_H