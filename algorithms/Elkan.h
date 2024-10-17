#ifndef ELKAN_H
#define ELKAN_H

#include "KMeansBase.h"
#include "../utils/Utils.h"

class Elkan: public KMeansBase {
public:
    std::vector<double> ub;
    std::vector<std::vector<double>> lb;
    std::vector<double> inner_bound;

public:
    Elkan(int max_iterations = MAX_ITERATIONS, double convergence_threshold = 0.001);

    void initParameters(int data_scale, int data_dimension, int k) override;

    void run() override;

protected:
    void assignLabels() override;
    
    void setInnerBound();

    void assignAndSetBound();
};

#endif  // ELKAN_H