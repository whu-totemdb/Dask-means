#include "Cluster.h"
#include "../utils/Utils.h"
#include <algorithm>
#include <vector>

using namespace Utils;

Cluster::Cluster(int cluster_id): cluster_id(cluster_id) {}

Cluster::~Cluster() {}

std::vector<int> Cluster::getDataIdList() {
    return data_id_list;
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
}

void Cluster::dataIn(std::vector<double> data_in, Node& node) {
    sum_vec = addVector(sum_vec, node.sum_vector);
    node_list.push_back(&node);
}

void Cluster::dataOut(std::vector<double> data_out, int data_id) {
    sum_vec = subtractVector(sum_vec, data_out);
    auto new_end = std::remove(data_id_list.begin(), data_id_list.end(), data_id);
    data_id_list.erase(new_end, data_id_list.end());
}

void Cluster::dataOut(std::vector<double> data_out, Node& node) {
    sum_vec = subtractVector(sum_vec, data_out);
    auto new_end = std::remove(node_list.begin(), node_list.end(), node);
    node_list.erase(new_end, node_list.end());
}