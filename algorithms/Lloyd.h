#ifndef LLOYD_H
#define LLOYD_H

#include "KMeansBase.h"
#include "../utils/utils.h"

class Lloyd : public KMeansBase {
public:
    Lloyd(int max_iterations = 30, double convergence_threshold = 0.001);

    void run() override;

protected:
    void assignLabels() override;

    void updateCentroids() override;
};

#endif // LLOYD_H