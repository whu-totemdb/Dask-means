#include "DaskMeans.h"

DaskMeans::DaskMeans(int max_iterations, double convergence_threshold)
    : KMeansBase(max_iterations, convergence_threshold) {
    inner_bound = std::vector<double>(k, 0.0);
}

DaskMeans::~DaskMeans() {
    for (auto centroid : centroid_list) {
        delete centroid;
    }
    delete data_index;
    delete centroid_index;
}



void DaskMeans::buildDataIndex(int capacity) {
    data_index = new BallTree(capacity);
    data_index->buildBallTree(dataset, data_scale);
}

void DaskMeans::buildCentroidIndex(int capacity) {
    if (centroid_index != nullptr) {
        delete centroid_index;
    }
    centroid_index = new BallTree(capacity);
    centroid_index->buildBallTree(centroid_list, k);
}