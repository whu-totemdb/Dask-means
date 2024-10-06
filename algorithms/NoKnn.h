// NoKnn.h
#ifndef NOKNN_H
#define NOKNN_H

#include "DaskMeans.h"

class NoKnn : public DaskMeans {
public:
    NoKnn(int capacity, int max_iterations = 30, double convergence_threshold = 0.001);
    ~NoKnn();

    void run() override;

protected:
    void setInnerBound();

    void assignLabels(Node& node, double ub);

};

#endif