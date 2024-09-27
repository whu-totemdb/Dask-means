// KMeansBase.h
#ifndef KMEANSBASE_H
#define KMEANSBASE_H

#include <vector>
#include <algorithm>
#include <string>

#include "cluster.h"
#include "centroid.h"

class KMeansBase {
protected:
    std::vector<std::vector<double>> dataset;
    std::vector<Centroid*> centroid_list;   // remember to release the memory
    std::vector<int> labels;    // noting which cluster the data point is assigned to
    int data_scale;
    int data_dimension;
    int k;
    int max_iterations;
    double convergence_threshold;

public:
    KMeansBase(int max_iterations = 30, double convergence_threshold = 1e-4);

    virtual ~KMeansBase() {}

    virtual void initParameters(int data_scale, int data_dimension, int k);

    void setK(int k); 

    void load(const std::string& file_path);

    virtual void run() = 0;

    virtual std::vector<int> output() {}

protected:
    virtual void initializeCentroids();

    virtual void assignLabels() {}  // assign all data points to a cluster with an intiger label

    virtual void updateCentroids() {}

    bool hasConverged(const std::vector<std::vector<double>>& newCentroids) { return false; }
};

#endif // KMEANSBASE_H