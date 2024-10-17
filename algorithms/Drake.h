#ifndef DRAKE_H
#define DRAKE_H

#include "KMeansBase.h"
#include "../utils/Utils.h"
#include "../structure/KnnRes.h"

class Drake: public KMeansBase {
public:
    int b;
    std::vector<double> ub;
    std::vector<std::vector<KnnRes*>> a; // keep b nearest centroids for each point
    double max_drift;
    int max_b;

public:
    Drake(int max_iterations = MAX_ITERATIONS, double convergence_threshold = 0.001);

    void initParameters(int data_scale, int data_dimension, int k) override;

    void run() override;

protected:
    void assignLabels() override;

    void updateCentroids() override;

    void sortCentroids(int point_id, int q, std::vector<Centroid*> cent_list);
    void sortCentroids(int point_id, int q, std::vector<Centroid*> cent_list,
            std::vector<KnnRes*> res);
};

#endif  // DRAKE_H