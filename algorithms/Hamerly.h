#ifndef HAMERLY_H
#define HAMERLY_H

#include "KMeansBase.h"
#include "../utils/Utils.h"

class Hamerly: public KMeansBase {
public:
    std::vector<double> ub;
    std::vector<double> lb;
    double max_drift;

public:
    Hamerly(int max_iterations = MAX_ITERATIONS, double convergence_threshold = 0.001);

    void run() override;

protected:
    void assignLabels() override;

    void updateCentroids() override;
};

#endif  // HAMERLY_H