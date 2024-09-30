#include <BallTree.h>
#include <../utils/Utils.h>

using namespace Utils;

void BallTree::buildBallTree(std::vector<std::vector<double>>& dataset, int data_scale) {
    createNode(dataset, data_scale, *root);
    std::vector<int> point_id_list(data_scale);
    for (int i = 0; i < data_scale; i++) {
        point_id_list[i] = i;
    }
    buildBallTree1(dataset, *root, point_id_list);
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

void BallTree::buildBalancedBallTree(std::vector<std::vector<double>>& dataset, 
    int data_scale, int data_dimension) {
    
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


// find two farthest point to the center in dataset
std::vector<int> BallTree::getTwoFarthestPoints(const std::vector<double>& center, 
    const std::vector<std::vector<double>>& dataset, int data_scale) {

    std::vector<int> point_ids(2, 0);       // [farthest point id, second ... id]
    std::vector<double> distances(2, 0.0);  // [farthest distance, second ...]
    for (int point_id = 0; point_id < data_scale; point_id++) {
        double dis = distance1(center, dataset[point_id]);

        if (dis >= distances[0]) {
            point_ids[1] = point_ids[0];
            point_ids[0] = point_id;
            distances[1] = distances[0];
            distances[0] = dis;
        } 
        else if (dis > distances[1]) {
            point_ids[1] = point_id;
            distances[1] = dis;
        }
    }
    return point_ids;
}

std::vector<int> BallTree::getTwoFarthestPoints(const std::vector<double>& center, 
    const std::vector<std::vector<double>>& dataset, std::vector<int> point_id_list) {

    std::vector<int> point_ids(2, 0);       // [farthest point id, second ... id]
    std::vector<double> distances(2, 0.0);  // [farthest distance, second ...]
    for (int point_id : point_id_list) {
        double dis = distance1(center, dataset[point_id]);

        if (dis >= distances[0]) {
            point_ids[1] = point_ids[0];
            point_ids[0] = point_id;
            distances[1] = distances[0];
            distances[0] = dis;
        } 
        else if (dis > distances[1]) {
            point_ids[1] = point_id;
            distances[1] = dis;
        }
    }
    return point_ids;
}