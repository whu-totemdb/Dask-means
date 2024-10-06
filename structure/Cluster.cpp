#include "Cluster.h"
#include "../utils/Utils.h"
#include <algorithm>
#include <vector>

using namespace Utils;

Cluster::Cluster(int cluster_id, int data_dimension): cluster_id(cluster_id) {
    this->sum_vec = std::vector<double>(data_dimension, 0.0);
}

Cluster::~Cluster() {}

std::vector<int> Cluster::getDataIdList() {
    return data_id_list;
}

std::vector<int> Cluster::getAllDataId() {
    std::vector<int> all_data_id = data_id_list;
    for (auto node : node_list) {
        std::vector<int> data_in_node = node->getAllDataId();
        all_data_id.insert(all_data_id.end(), data_in_node.begin(), data_in_node.end());
    }
    return all_data_id;
}

void Cluster::addDataId(int data_id) {
    data_id_list.push_back(data_id); 
}

int Cluster::getClusterId() { 
    return cluster_id;
}

void Cluster::clear() { 
    data_id_list.clear(); 
}

void Cluster::dataIn(std::vector<double> data_in, int data_id) {
    sum_vec = addVector(sum_vec, data_in);
    data_id_list.push_back(data_id);
    point_number += 1;
}

void Cluster::dataIn(std::vector<double> data_in, Node* node) {
    sum_vec = addVector(sum_vec, node->sum_vector);
    node_list.push_back(node);
    point_number += node->point_number;
}

void Cluster::dataOut(std::vector<double> data_out, int data_id) {
    auto it = std::find(data_id_list.begin(), data_id_list.end(), data_id);
    if (it != data_id_list.end()) {
        auto new_end = std::remove(data_id_list.begin(), data_id_list.end(), data_id);
        data_id_list.erase(new_end, data_id_list.end());

        sum_vec = subtractVector(sum_vec, data_out);
        point_number -= 1;
    }
}

void Cluster::dataOut(std::vector<double> data_out, Node* node) {
    auto it = std::find(node_list.begin(), node_list.end(), node);
    if (it != node_list.end()) {
        auto new_end = std::remove(node_list.begin(), node_list.end(), node);
        node_list.erase(new_end, node_list.end());

        sum_vec = subtractVector(sum_vec, data_out);
        point_number -= node->point_number;
    }
}