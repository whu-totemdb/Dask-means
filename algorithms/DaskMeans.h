// DaskMeans.h
#ifndef DASKMEANS_H
#define DASKMEANS_H

#include <vector>
#include "KMeansBase.h"
#include "../utils/Utils.h"
#include "../structure/BallTree.h"
#include "../structure/KnnRes.h"

using namespace Utils;

class DaskMeans : public KMeansBase {
private:
    BallTree* data_index;
    BallTree* centroid_index;
    std::vector<double> inner_bound;
    std::vector<double> ub;
    int capacity;
    /*
    std::vector<std::vector<double>> dataset;
    std::vector<Centroid*> centroid_list;   // remember to release the memory
    std::vector<int> labels;    // noting which cluster the data point is assigned to
    int data_scale;
    int data_dimension;
    int k;
    int max_iterations;
    double convergence_threshold;
    */

public:
    DaskMeans(int capacity, int max_iterations = 30, double convergence_threshold = 0);

    ~DaskMeans() override;

    void run();

    void output(const std::string& file_path) override;

protected:
    void buildDataIndex(int capacity = 1);

    void buildCentroidIndex(int capacity = 1);

    void setInnerBound();

    void assignLabels(Node& node, double ub);
    
    void updateCentroids() override;
    
};

#endif