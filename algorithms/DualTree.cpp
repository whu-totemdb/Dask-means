#include "DualTree.h"
#include "../structure/KdTree.h"
#include "../structure/KnnRes.h"
#include "../utils/Utils.h"

using namespace Utils;

DualTree::DualTree(int capacity, int max_iterations, double convergence_threshold)
    : KMeansBase(max_iterations, convergence_threshold) {
        this->capacity = capacity;
        max_drift = 0;
    }

DualTree::~DualTree() {
    delete data_index;
    delete centroid_index;
    data_index = nullptr;
    centroid_index = nullptr;
}

void DualTree::initParameters(int data_scale, int data_dimension, int k) {
    this->data_scale = data_scale;
    this->data_dimension = data_dimension;
    this->k = k;
    this->labels.assign(this->data_scale, 0);
    point_ub = std::vector(data_scale, std::numeric_limits<double>::max());
    point_lb = std::vector(data_scale, std::numeric_limits<double>::max());
}

void DualTree::run() {
    buildDataIndex(k, this->capacity);
    initializeCentroids();
    do {
        buildCentroidIndex(k);
        setInnerBound();
        assignLabels(*data_index->root);
        updateCentroids();
        output("/home/lzp/cs/dask-means-cpp/output/DualTree_output.txt");
    } while (!hasConverged());
}

void DualTree::buildDataIndex(int k, int capacity) {
    data_index = new KdTree(k, capacity);
    data_index->buildKdTree(dataset, data_scale);
}

void DualTree::buildCentroidIndex(int k, int capacity) {
    if (centroid_index != nullptr) {
        delete centroid_index;
        centroid_index = nullptr;
    }
    centroid_index = new KdTree(k, capacity);
    centroid_index->buildKdTree(centroid_list, k);
}

void DualTree::setInnerBound() {
    this->inner_bound = std::vector<double>(k, -1.0);
    for (int i = 0; i < k; i++) {
        std::vector<KnnRes*> res(2);
        for (int i = 0; i < 2; i++) {
            res[i] = new KnnRes();
        }
        calculate2nn(centroid_list[i]->getCoordinate(), res, centroid_list);
        inner_bound[i] = res[1]->dis;
    }
}

void DualTree::assignLabels(KdTreeNode& node) {
    // 1.if the node is assigned before
    if (node.centroid_id != -1) {
        // update ub and lb
        node.ub += centroid_list[node.centroid_id]->drift;
        node.lb -= max_drift;

        // strategy one and strategy three
        // if (node.ub <= node.lb)
        //     return;
        // if (node.ub < inner_bound[node.centroid_id] / 2)
        //     return;

        
        node.resetBound(dataset, *centroid_index->root, centroid_list);
        // if (node.ub <= node.lb)
        //     return;
        if (node.ub < inner_bound[node.centroid_id] / 2)
            return;
    }
    // else assign all data point covered by the node, and then set assignment
    if (!node.leaf) {
        assignLabels(*node.leftChild);
        assignLabels(*node.rightChild);
    } else {
        for (int id = 0; id < node.data_id_list.size(); id++) {
            assignPoint(node, id);
        }
    }
    setAssignment(node);
    if (node.centroid_id != -1) {
        node.resetBound(dataset, *centroid_index->root, centroid_list);
    }
}

void DualTree::updateCentroids() {
    // clear cluster
    for (auto cent : centroid_list) {
        cent->cluster->clear();
    }

    // assign all point to a cluster
    updateCluster(*data_index->root);

    for (auto centroid : centroid_list) {
        std::vector<int> data_id_list = centroid->getCluster()->getDataIdList();
        int data_num = data_id_list.size();

        std::vector<double> new_coordinate(data_dimension, 0);
        for (int data_id : data_id_list) {
            new_coordinate = addVector(new_coordinate, dataset[data_id]);
        }
        new_coordinate = divideVector(new_coordinate, data_num);

        centroid->updateCoordinate(new_coordinate);

        max_drift = std::max(centroid->max_drift, max_drift);
    }
}

// assign all data points covered by the node to the nearest centroid
void DualTree::assignPoint(KdTreeNode& node, int index) {
    int point_id = node.data_id_list[index];
    int centroid_id = node.centroid_id_for_data[index];

    if (centroid_id != -1) {
        // update up and lb
        point_ub[point_id] += centroid_list[centroid_id]->drift;
        point_lb[point_id] -= max_drift;

        // if (point_ub[point_id] <= point_lb[point_id] ||
        //         point_ub[point_id] < inner_bound[node.centroid_id] / 2) {
        //     return;
        // }
    }

    double nearest_dis = std::numeric_limits<double>::max();
    double second_nearest_dis = std::numeric_limits<double>::max();
    int nearest_id = -1;
    for (int i = 0; i < k; i++) {
        double dis = distance1(dataset[point_id], centroid_list[i]->getCoordinate());

        if (dis <= nearest_dis) {
            second_nearest_dis = nearest_dis;
            nearest_dis = dis;
            nearest_id = i;
        } else if (dis < second_nearest_dis) {
            second_nearest_dis = dis;
        }
    }
    // set point ub and lb
    point_lb[point_id] = second_nearest_dis;
    point_ub[point_id] = nearest_dis;

    node.centroid_id_for_data[index] = nearest_id;
}

// assign a node to a cluster if all children (or data points) are assigned to the same cluster
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

void DualTree::updateCluster(KdTreeNode& node) {
    if (node.leaf) {
        for (int i = 0; i < node.point_number; i++) {
            Cluster* clu = centroid_list[node.centroid_id_for_data[i]]->cluster;
            clu->data_id_list.push_back(node.data_id_list[i]);
        }
        return;
    }
    updateCluster(*node.leftChild);
    updateCluster(*node.rightChild);
}