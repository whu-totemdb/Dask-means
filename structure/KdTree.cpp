#include "KdTree.h"
#include <algorithm>
#include "../utils/Utils.h"

KdTree::KdTree(int dimensions, int capacity)
        : capacity(capacity), data_dimension(dimensions) {
    root = new KdTreeNode();
}

KdTree::~KdTree() {
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }
}

void KdTree::buildKdTree(std::vector<std::vector<double>>& dataset, int data_scale) {
    std::vector<int> point_id_list(data_scale);
    for (int i = 0; i < data_scale; i++) {
        point_id_list[i] = i;
    }
    buildKdTree1(*root, dataset, point_id_list);
}

void KdTree::buildKdTree(std::vector<Centroid*>& centroid_list, int k) {
    std::vector<int> centroid_id_list(k);
    for (int i = 0; i < k; i++) {
        centroid_id_list[i] = i;
    }
    buildKdTree1(*root, centroid_list, centroid_id_list);
}

void KdTree::buildKdTree1(KdTreeNode& node, std::vector<std::vector<double>>& dataset, 
        std::vector<int>& point_id_list) {
    // is leaf node
    int size = point_id_list.size();
    if (size <= capacity) {
        node.initLeafNode(dataset, point_id_list, size);
        return;
    }

    // 1.find the dimension with the maximum variance
    node.current_dimension = Utils::findBestDimension(dataset, point_id_list);
    int axis = node.current_dimension;

    // 2.sort all point by the dimension with the greatest variance
    std::sort(point_id_list.begin(), point_id_list.end(), 
              [&dataset, axis](int a, int b) {
                  return dataset[a][axis] < dataset[b][axis];
              });

    // 3.split the point_id_list into two parts
    int median_index = size / 2 - 1;
    node.split_point = dataset[point_id_list[median_index]];

    for (int id : point_id_list) {
        node.r = std::max(node.r, Utils::distance1(node.split_point, dataset[id]));
    }

    std::vector<int> left_points_ids(point_id_list.begin(), point_id_list.begin() + median_index + 1);
    std::vector<int> right_points_ids(point_id_list.begin() + median_index + 1, point_id_list.end());

    node.leftChild = new KdTreeNode();
    node.rightChild = new KdTreeNode();
    buildKdTree1(*(node.leftChild), dataset, left_points_ids);
    buildKdTree1(*(node.rightChild), dataset, right_points_ids);
}

void KdTree::buildKdTree1(KdTreeNode& node, std::vector<Centroid*>& centroid_list, 
        std::vector<int> centroid_id_list) {
    // is leaf node
    int size = centroid_id_list.size();
    if (size <= capacity) {
        node.initLeafNode(centroid_id_list);
        return;
    }

    // 1.find the dimension with the maximum variance
    node.current_dimension = Utils::findBestDimension(centroid_list, centroid_id_list);
    int axis = node.current_dimension;

    // 2.sort all point by the dimension with the greatest variance
    std::sort(centroid_id_list.begin(), centroid_id_list.end(), 
              [&centroid_list, axis](int a, int b) {
                  return centroid_list[a]->getCoordinate()[axis] < centroid_list[b]->getCoordinate()[axis];
              });

    // 3.split the point_id_list into two parts
    int median_index = size / 2 - 1;
    node.split_point = centroid_list[centroid_id_list[median_index]]->getCoordinate();

    std::vector<int> left_points_ids(centroid_id_list.begin(), centroid_id_list.begin() + median_index + 1);
    std::vector<int> right_points_ids(centroid_id_list.begin() + median_index + 1, centroid_id_list.end());

    node.leftChild = new KdTreeNode();
    node.rightChild = new KdTreeNode();
    buildKdTree1(*(node.leftChild), centroid_list, left_points_ids);
    buildKdTree1(*(node.rightChild), centroid_list, right_points_ids);
}