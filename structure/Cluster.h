// cluster.h
#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include "Node.h"

class Cluster {
public:
    std::vector<int> data_id_list;
    int cluster_id;

    // used for dask-means
    std::vector<Node*> node_list;
    int point_number = 0;
    std::vector<double> sum_vec;

    // for debug
    // double data_in_time = 0.0;
    // double data_out_time = 0.0;

public:
    Cluster(int cluster_id, int data_dimension);

    ~Cluster();

    std::vector<int> getDataIdList();
    std::vector<int> getAllDataId();    // for dask-means

    void addDataId(int data_id);

    int getClusterId();

    void clear();

    void dataIn(std::vector<double> data_in, int data_id);
    void dataIn(std::vector<double> data_in, Node* node);
    void dataIn(int point_num, std::vector<double> data_in);

    void dataOut(std::vector<double> data_out, int data_id);
    void dataOut(std::vector<double> data_out, Node* node);
    void dataOut(int point_num, std::vector<double> data_out);
};

#endif // CLUSTER_H