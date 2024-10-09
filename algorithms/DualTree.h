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
    DualTree(int capacity, int max_iterations = 30, double convergence_threshold = 0.001);
    
    ~DualTree() override;

    void run() override;

    void output(const std::string& file_path) override;

protected:
    void buildDataIndex(int capacity = 1);

    void buildCentroidIndex(int capacity = 1);

    void setInnerBound();

    void assignLabels(KdTreeNode& node);
    
    void updateCentroids() override;

    void assignToCluster(KdTreeNode& node, int centroid_id);

    void assignPoint(KdTreeNode& node, int index);

    void setAssignment(KdTreeNode& node);

    void updateMaxDrift();
};

#endif