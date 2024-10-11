#include <BallTree.h>
#include <../utils/Utils.h>

using namespace Utils;


BallTree::BallTree() { root = new Node(); }

BallTree::BallTree(int capacity): capacity(capacity) { root = new Node(); }

BallTree::~BallTree() {
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }
}

void BallTree::buildBallTree(std::vector<std::vector<double>>& dataset, int data_scale) {
    createNode(dataset, data_scale, *root);
    std::vector<int> point_id_list(data_scale);
    for (int i = 0; i < data_scale; i++) {
        point_id_list[i] = i;
    }
    buildBallTree1(dataset, *root, point_id_list);
    initBallTree();
}

void BallTree::buildBallTree(std::vector<Centroid*>& centroid_list, int k) {
    createNode(centroid_list, k, *root);
    std::vector<int> centroid_id_list(k);
    for (int i = 0; i < k; i++) {
        centroid_id_list[i] = i;
    }
    buildBallTree1(centroid_list, *root, centroid_id_list);
}


/**
 * @brief build a ball-tree recursively
 * @param dataset the full dataset loaded by the algorithm
 * @param node current ball-tree node that will be constructed
 * @param point_id_list the list of points' ids covered by current node
 */
void BallTree::buildBallTree1(std::vector<std::vector<double>>& dataset, 
    Node& node, std::vector<int> point_id_list) {

    int size = point_id_list.size();
    if (size <= this->capacity) {
        node.initLeafNode(point_id_list, size);
        return;
    }
    // is not leaf node
    // 1. split the point_id_list into two list
    std::vector<double> vec1 = dataset[node.farthest_point_id[0]];
    std::vector<double> vec2 = dataset[node.farthest_point_id[1]];
    std::vector<int> point_id_list1;
    std::vector<int> point_id_list2;

    for (int point_id : point_id_list) {
        std::vector<double> point = dataset[point_id];
        if (distance2(vec1, point) <= distance2(vec2, point)) {
            point_id_list1.push_back(point_id);
        } else {
            point_id_list2.push_back(point_id);
        }
    }

    // 2. create two child nodes
    int size1 = point_id_list1.size();
    int size2 = point_id_list2.size();
    node.leftChild = new Node();
    node.rightChild = new Node();
    createNode(dataset, point_id_list1, size1, *(node.leftChild));
    createNode(dataset, point_id_list2, size2, *(node.rightChild));

    buildBallTree1(dataset, *(node.leftChild), point_id_list1);
    buildBallTree1(dataset, *(node.rightChild), point_id_list2);
}

void BallTree::buildBallTree1(std::vector<Centroid*>& centroid_list, 
    Node& node, std::vector<int> centroid_id_list) {

    int size = centroid_id_list.size();
    if (size <= this->capacity) {
        node.initLeafNode(centroid_id_list, size);
        return;
    }
    // is not leaf node
    // 1. split the point_id_list into two list
    std::vector<double> vec1 = centroid_list[node.farthest_point_id[0]]->getCoordinate();
    std::vector<double> vec2 = centroid_list[node.farthest_point_id[1]]->getCoordinate();
    std::vector<int> centroid_id_list1;
    std::vector<int> centroid_id_list2;

    for (int centroid_id : centroid_id_list) {
        std::vector<double> coordinate = centroid_list[centroid_id]->getCoordinate();
        if (distance2(vec1, coordinate) <= distance2(vec2, coordinate)) {
            centroid_id_list1.push_back(centroid_id);
        } else {
            centroid_id_list2.push_back(centroid_id);
        }
    }

    // 2. create two child nodes
    int size1 = centroid_id_list1.size();
    int size2 = centroid_id_list2.size();
    node.leftChild = new Node();
    node.rightChild = new Node();
    createNode(centroid_list, centroid_id_list1, size1, *(node.leftChild));
    createNode(centroid_list, centroid_id_list2, size2, *(node.rightChild));

    buildBallTree1(centroid_list, *(node.leftChild), centroid_id_list1);
    buildBallTree1(centroid_list, *(node.rightChild), centroid_id_list2);
}

void BallTree::buildBalancedBallTree(std::vector<std::vector<double>>& dataset, 
    int data_scale, int data_dimension) {
    
}

void BallTree::initBallTree() {
    root->setSum();
}

/*function*/

// giving a dataset and point_id_list, return a ball-tree node with pivot and radius
void BallTree::createNode(std::vector<std::vector<double>>& dataset, int data_scale, Node& node) {
    std::vector<double> pivot = sumVectorsInDataset(dataset);
    pivot = divideVector(pivot, data_scale);
    std::vector<int> farthest_point_id = getTwoFarthestPoints(pivot, dataset, data_scale);
    double radius = distance1(pivot, dataset[farthest_point_id[0]]);

    node.pivot = pivot;
    node.radius = radius;
    node.farthest_point_id = farthest_point_id;
}

void BallTree::createNode(std::vector<std::vector<double>>& dataset, 
    std::vector<int> point_id_list, int size, Node& node) {

    std::vector<double> pivot = sumVectorsInDataset(dataset, point_id_list);
    pivot = divideVector(pivot, size);
    std::vector<int> farthest_point_id = getTwoFarthestPoints(pivot, dataset, point_id_list);
    double radius = distance1(pivot, dataset[farthest_point_id[0]]);

    node.pivot = pivot;
    node.radius = radius;
    node.farthest_point_id = farthest_point_id;
}

void BallTree::createNode(std::vector<Centroid*>& centroid_list, int data_scale, Node& node) {
    std::vector<double> pivot = sumVectorsInDataset(centroid_list);
    pivot = divideVector(pivot, data_scale);
    std::vector<int> farthest_point_id = getTwoFarthestPoints(pivot, centroid_list, data_scale);
    double radius = distance1(pivot, centroid_list[farthest_point_id[0]]->getCoordinate());

    node.pivot = pivot;
    node.radius = radius;
    node.farthest_point_id = farthest_point_id;
}

void BallTree::createNode(std::vector<Centroid*>& centroid_list, 
    std::vector<int> centroid_id_list, int size, Node& node) {

    std::vector<double> pivot = sumVectorsInDataset(centroid_list, centroid_id_list);
    pivot = divideVector(pivot, size);
    std::vector<int> farthest_point_id = getTwoFarthestPoints(pivot, centroid_list, centroid_id_list);
    double radius = distance1(pivot, centroid_list[farthest_point_id[0]]->getCoordinate());

    node.pivot = pivot;
    node.radius = radius;
    node.farthest_point_id = farthest_point_id;
}
