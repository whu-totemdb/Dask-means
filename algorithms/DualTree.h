// DualTree.h
#ifndef DUALTREE_H
#define DUALTREE_H

#include "KMeansBase.h"
#include "../structure/KdTree.h"

class DualTree : public KMeansBase {
protected:
    KdTree* data_index;
    KdTree* centroid_index;
    std::vector<double> inner_bound;
    int capacity;
    std::vector<double> point_ub;
    std::vector<double> point_lb;
    double max_drift;

public:
    DualTree(int capacity, int max_iterations = MAX_ITERATIONS, double convergence_threshold = 0.001);
    
    ~DualTree() override;

    void initParameters(int data_scale, int data_dimension, int k) override;

    void run() override;

protected:
    void buildDataIndex(int k, int capacity);

    void buildCentroidIndex(int k, int capacity = 1);

    void setInnerBound();

    void assignLabels(KdTreeNode& node);
    
    void updateCentroids() override;

    void assignPoint(KdTreeNode& node, int index);

    void setAssignment(KdTreeNode& node);

    void updateMaxDrift();

    void updateCluster(KdTreeNode& node);
};

#endif