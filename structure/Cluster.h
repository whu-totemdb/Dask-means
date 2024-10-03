// cluster.h
#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>

class Cluster {
public:
    std::vector<int> data_id_list;
    int cluster_id;

    // used for dask-means
    std::vector<Node*> node_list;
    std::vector<double> sum_vec;

public:
    Cluster(int cluster_id);

    ~Cluster();

    std::vector<int> getDataIdList();

    void addDataId(int data_id);

    int getClusterId();

    void clear();

    void dataIn(std::vector<double> data_in, int data_id);
    void dataIn(std::vector<double> data_in, Node& node);

    void dataOut(std::vector<double> data_out, int data_id);
    void dataOut(std::vector<double> data_out, Node& node);
};

#endif // CLUSTER_H