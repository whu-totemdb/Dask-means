#ifndef YINYANG_H
#define YINYANG_H

#include "KMeansBase.h"
#include "../utils/Utils.h"

class Group {
public:
    std::vector<int> centroid_id_list;
    double max_drift;

    Group() { max_drift = 0.0; }
};

class Yinyang : public KMeansBase {
public:
    std::vector<double> ub;         // upper bound for each point, denoting the nearest distance
    std::vector<double> global_lb;  // global lower bound for each point
    double max_drift;

    int group_num;                  // group number in Yinyang k-means
    std::vector<std::vector<double>> group_lb;  // group lower bound for each point
    std::vector<Group*> groups;
    

public:
    Yinyang(int max_iterations = MAX_ITERATIONS, double convergence_threshold = 0.001);

    void run() override;

protected:
    void assignLabels() override;

    void updateCentroids() override;

    void classicKMeansAssign();

    void classicKMeansUpdate();

    void setGroups();
};

#endif // YINYANG_H