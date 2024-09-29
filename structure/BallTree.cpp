#include <BallTree.h>
#include <../utils/Utils.h>

using namespace Utils;

void BallTree::buildBallTree(std::vector<std::vector<double>>& dataset, 
    int data_scale, int data_dimension) {
    
    std::vector<double> pivot = sumVectorsInDataset(dataset);
    pivot = divideVector(pivot, data_scale);
    std::vector<int> farthest_point_id = getTwoFarthestPoints(pivot, dataset, data_scale);
    double radius = distance1(pivot, dataset[farthest_point_id[0]]);
    root = new Node(pivot, radius);

    std::vector<int> point_id_list(data_scale);
    for (int i = 0; i < data_scale; i++) {
        point_id_list[i] = i;
    }
    buildBallTree1(dataset, root, point_id_list, farthest_point_id);
}

/**
 * @brief build a ball-tree recursively
 * @param dataset the full dataset loaded by the algorithm
 * @param node current ball-tree node that will be constructed
 * @param point_id_list the list of points' ids covered by current node
 * @param farthest_point_id two farthest points' id
 */
void BallTree::buildBallTree1(std::vector<std::vector<double>>& dataset, Node* node, 
    std::vector<int> point_id_list, std::vector<int> farthest_point_id) {

    int size = point_id_list.size();
    if (size <= this->capacity) {
        node->initLeafNode(point_id_list, size);
        return;
    }
    // is not leaf node
    // 1. split the point_id_list into two list
    std::vector<double> vec1 = dataset[farthest_point_id[0]];
    std::vector<double> vec2 = dataset[farthest_point_id[1]];
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
    std::vector<double> pivot1 = sumVectorsInDataset(dataset, point_id_list1);
    std::vector<double> pivot2 = sumVectorsInDataset(dataset, point_id_list2);
    pivot1 = divideVector(pivot1, size1);
    pivot2 = divideVector(pivot2, size2);

    std::vector<int> farthest_point_id1 = getTwoFarthestPoints(pivot1, dataset, point_id_list1);
    std::vector<int> farthest_point_id2 = getTwoFarthestPoints(pivot2, dataset, point_id_list2);
    double radius1 = distance1(pivot1, dataset[farthest_point_id1[0]]);
    double radius2 = distance1(pivot2, dataset[farthest_point_id2[0]]);
    node->setLeftChild(new Node(pivot1, radius1));
    node->setRightChild(new Node(pivot2, radius2));

    buildBallTree1(dataset, node->getLeftChild(), point_id_list1, farthest_point_id1);
    buildBallTree1(dataset, node->getRightChild(), point_id_list2, farthest_point_id2);
}

void BallTree::buildBalancedBallTree(std::vector<std::vector<double>>& dataset, 
    int data_scale, int data_dimension) {
    
}


// function

// giving a dataset and point_id_list, return a ball-tree node with pivot and radius
Node* createNode() {

}


// find two farthest point to the center in dataset
std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
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

std::vector<int> getTwoFarthestPoints(const std::vector<double>& center, 
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