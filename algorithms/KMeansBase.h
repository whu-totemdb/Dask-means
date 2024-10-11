// KMeansBase.h
#ifndef KMEANSBASE_H
#define KMEANSBASE_H

#define MAX_ITERATIONS 20

#include <vector>
#include <algorithm>
#include <string>

#include "../structure/Centroid.h"
#include "../structure/Cluster.h"

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
    std::vector<double> runtime;

public:
    KMeansBase(int max_iterations = MAX_ITERATIONS, double convergence_threshold = 1e-4);

    virtual ~KMeansBase();

    virtual void initParameters(int data_scale, int data_dimension, int k);

    void setK(int k); 

    void load(const std::string& file_path);

    virtual void run() = 0;

    virtual void output(const std::string& file_path);

protected:
    virtual void initializeCentroids();

    virtual void assignLabels();

    virtual void updateCentroids();

    virtual bool hasConverged();
};

#endif