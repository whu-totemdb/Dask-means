// centroid.h
#ifndef CENTROID_H
#define CENTROID_H

#include <vector>
#include "cluster.h"

class Centroid {
protected:
    std::vector<double> coordinate;
    std::vector<double> old_coordinate; // coordinate from the last iteration
    int point_id;       // only used in initialization
    int centroid_id;
    Cluster* cluster;   // remember to release the memory

public:
    Centroid(int point_id, std::vector<double> coordinate, int centroid_id)
        : point_id(point_id), coordinate(coordinate), centroid_id(centroid_id) {
        cluster = new Cluster(centroid_id);
    }

    ~Centroid() { delete cluster; }

    std::vector<double> getCoordinate() { return coordinate; }

    std::vector<double> getOldCoordinate() { return old_coordinate; }

    Cluster* getCluster() { return cluster; }

    void updateCoordinate(std::vector<double> new_coordinate) {
        this->old_coordinate = this->coordinate;
        this->coordinate = new_coordinate;
    }
};

#endif // CENTROID_H