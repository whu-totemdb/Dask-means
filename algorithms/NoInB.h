// NoInB.h
#ifndef NOINB_H
#define NOINB_H

#include "DaskMeans.h"

class NoInB : public DaskMeans {
public:
    NoInB(int capacity, int max_iterations = MAX_ITERATIONS, double convergence_threshold = 0.001);
    ~NoInB();

    void run() override;

protected:
    void assignLabels(Node& node, double ub);

};


#endif