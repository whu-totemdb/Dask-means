// cluster.h
#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>

class Cluster {
protected:
    std::vector<int> data_id_list;
    int cluster_id;

public:
    Cluster(int cluster_id): cluster_id(cluster_id) {}

    ~Cluster() {}

    std::vector<int> getDataIdList() { return data_id_list; }

    void addDataId(int data_id) { data_id_list.push_back(data_id); }

    int getClusterId() { return cluster_id; }

    void clear() { data_id_list.clear(); }
};

#endif // CLUSTER_H