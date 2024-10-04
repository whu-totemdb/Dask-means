// centroid.h
#ifndef CENTROID_H
#define CENTROID_H

#include <vector>
#include "Cluster.h"

class Centroid {
public:
    std::vector<double> coordinate;
    std::vector<double> old_coordinate; // coordinate from the last iteration
    int point_id;       // only used in initialization
    int centroid_id;
    Cluster* cluster;   // remember to release the memory

    // for dask-means
    double drift;
    double max_drift;

public:
    Centroid(int point_id, std::vector<double> coordinate, int centroid_id);

    ~Centroid();

    std::vector<double> getCoordinate();

    std::vector<double> getOldCoordinate();

    Cluster* getCluster();

    void updateCoordinate(std::vector<double> new_coordinate);
};

#endif // CENTROID_H