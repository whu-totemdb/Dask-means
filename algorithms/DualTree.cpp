#include "DualTree.h"
#include "../structure/KdTree.h"
#include "../structure/KnnRes.h"
#include "../utils/Utils.h"

using namespace Utils;

DualTree::DualTree(int capacity, int max_iterations, double convergence_threshold)
    : KMeansBase(max_iterations, convergence_threshold) {
        this->capacity = capacity;
        point_ub = std::vector(data_scale, std::numeric_limits<double>::max());
        point_lb = std::vector(data_scale, std::numeric_limits<double>::max());
        max_drift = 0;
    }

DualTree::~DualTree() {
    delete data_index;
    delete centroid_index;
    data_index = nullptr;
    centroid_index = nullptr;
}

void DualTree::run() {

}

void DualTree::output(const std::string& file_path) {

}

void DualTree::buildDataIndex(int capacity) {
    data_index = new KdTree(capacity);
    data_index->buildKdTree(dataset, data_scale);
}

void DualTree::buildCentroidIndex(int capacity) {
    if (centroid_index != nullptr) {
        delete centroid_index;
        centroid_index = nullptr;
    }
    centroid_index = new KdTree(capacity);
    centroid_index->buildKdTree(centroid_list, k);
}

void DualTree::setInnerBound() {
    this->inner_bound = std::vector<double>(k, -1.0);
    for (int i = 0; i < k; i++) {
        if (inner_bound[i] >= 0) {
            continue;
        }
        std::vector<KnnRes*> res(2);
        for (int i = 0; i < 2; i++) {
            res[i] = new KnnRes();
        }
        calculate2nn(centroid_list[i]->getCoordinate(), res, centroid_list);
        inner_bound[i] = res[1]->dis;
        inner_bound[res[1]->id] = res[1]->dis;
    }
}

void DualTree::assignLabels(KdTreeNode& node) {
    // 1.if the node is assigned before
    if (node.centroid_id != -1) {
        // update ub and lb
        node.ub += centroid_list[node.centroid_id]->drift;
        node.lb -= max_drift;

        // strategy one and strategy three
        if (node.ub <= node.lb || node.ub < inner_bound[node.centroid_id] / 2) {
            return;
        }
        
        node.resetBound(dataset, *centroid_index->root, centroid_list);
        if (node.ub <= node.lb || node.ub < inner_bound[node.centroid_id] / 2) {
            return;
        }

        if (!node.leaf) {
            assignLabels(*node.leftChild);
            assignLabels(*node.rightChild);
        } else {
            for (int id : node.data_id_list) {
                assignPoint(node, id);
            }
        }

    } else {
    // else assign all data point covered by the node, and then set assignment
        if (!node.leaf) {
            assignLabels(*node.leftChild);
            assignLabels(*node.rightChild);
        } else {
            for (int id : node.data_id_list) {
                assignPoint(node, id);
            }
        }
        setAssignment(node);
    }
}

void DualTree::assignPoint(KdTreeNode& node, int index) {
    int point_id = node.data_id_list[index];
    int centroid_id = node.centroid_id_for_data[index];

    if (centroid_id != -1) {
        // update up and lb
        point_ub[point_id] += centroid_list[centroid_id]->drift;
        point_lb[point_id] -= max_drift;

        if (point_ub[point_id] <= point_lb[point_id] ||
                point_ub[point_id] < inner_bound[node.centroid_id] / 2) {
            return;
        }
    }

    double nearest_dis = std::numeric_limits<double>::max();
    for (int i = 0; i < k; i++) {
        double dis = distance1(dataset[point_id], centroid_list[centroid_id]->getCoordinate());
        if (dis < nearest_dis) {
            // set point ub and lb
            point_lb[point_id] = nearest_dis;
            point_ub[point_id] = dis;
            // assign
            nearest_dis = dis;
            node.centroid_id_for_data[index] = i;
        }
    }
}

void DualTree::setAssignment(KdTreeNode& node) {
    if (!node.leaf) {
        if (node.leftChild->centroid_id == node.rightChild->centroid_id) {
            node.centroid_id = node.leftChild->centroid_id;
        } else {
            node.centroid_id = -1;
        }
        return;
    }

    for (size_t i = 0; i < node.centroid_id_for_data.size(); i++) {
        if (node.centroid_id_for_data[i] != node.centroid_id_for_data[0]) {
            node.centroid_id = -1;
            return;
        }
    }
    node.centroid_id = node.centroid_id_for_data[0];
}

void DualTree::updateMaxDrift() {
    for (auto cent : centroid_list) {
        max_drift = std::max(max_drift, cent->max_drift);
    }
}