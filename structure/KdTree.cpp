#include "KdTree.h"
#include <algorithm>

KdTree::KdTree(int dimensions) : data_dimension(dimensions) {
    root = new KdTreeNode();
}

KdTree::~KdTree() {
    delete root;
}

void KdTree::buildKdTree(std::vector<std::vector<double>>& dataset, int data_scale) {
    std::vector<int> point_id_list(data_scale);
    for (int i = 0; i < data_scale; i++) {
        point_id_list[i] = i;
    }
    buildKdTree1(*root, dataset, point_id_list);
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
    node.current_dimension = findBestDimension(dataset, point_id_list);
    int axis = node.current_dimension;

    // 2.sort all point by the dimension with the greatest variance
    std::sort(point_id_list.begin(), point_id_list.end(), 
              [&dataset, axis](int a, int b) {
                  return dataset[a][axis] < dataset[b][axis];
              });

    // 3.split the point_id_list into two parts
    int median_index = size / 2;
    node.split_threshold = dataset[point_id_list[median_index]][axis];

    std::vector<int> left_points_ids(point_id_list.begin(), point_id_list.begin() + median_index);
    std::vector<int> right_points_ids(point_id_list.begin() + median_index + 1, point_id_list.end());

    node.leftChild = new KdTreeNode();
    node.rightChild = new KdTreeNode();
    buildKdTree1(*(node.leftChild), dataset, left_points_ids);
    buildKdTree1(*(node.rightChild), dataset, right_points_ids);
}

int KdTree::findBestDimension(const std::vector<std::vector<double>>& dataset, 
        const std::vector<int>& point_id_list) {
    int num_dimensions = dataset[0].size();
    std::vector<double> variances(num_dimensions, 0.0);

    // get variance in each dimension
    for (int d = 0; d < num_dimensions; ++d) {
        double mean = 0.0;
        int count = point_id_list.size();

        // mean
        for (int index : point_id_list) {
            mean += dataset[index][d];
        }
        mean /= count;

        // variance
        double variance = 0.0;
        for (int index : point_id_list) {
            variance += (dataset[index][d] - mean) * (dataset[index][d] - mean);
        }
        variance /= count;

        variances[d] = variance;
    }

    int best_dimension = 0;
    double max_variance = variances[0];
    for (int d = 1; d < num_dimensions; ++d) {
        if (variances[d] > max_variance) {
            max_variance = variances[d];
            best_dimension = d;
        }
    }

    return best_dimension;
}
