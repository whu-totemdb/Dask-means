#include "Lloyd.h"
#include <limits>
#include <cmath>
#include <Utils.h>
#include <iostream>

using namespace Utils;

Lloyd::Lloyd(int max_iterations, double convergence_threshold)
    : KMeansBase(max_iterations, convergence_threshold) {}

void Lloyd::run() {
    int it = 0;     // iteration
    double start_time, end_time;
    start_time = clock();

    // main loop
    initializeCentroids();
    do {
        if (it != 0) {
            start_time = clock();
        }
        assignLabels();
        updateCentroids();

        end_time = clock();
        runtime[it] = double(end_time - start_time) * 1000 / CLOCKS_PER_SEC;
        std::cout << "iter: " << it << ", runtime: " << runtime[it] << " ms" << std::endl;
        it++;
    } while (!hasConverged() && it < max_iterations);

    // show total runtime
    double total_runtime = 0.0;
    for (size_t i = 0; i < max_iterations; i++) {
        total_runtime += runtime[i];
    }
    std::cout << "successfully run Lloyd in " << total_runtime << " ms" << std::endl;

}

void Lloyd::assignLabels() {
    // clear all clusters firstly
    for (auto centroid : centroid_list) {
        centroid->getCluster()->clear();
    }

    for (int j = 0; j < data_scale; j++) {
        auto data_point = dataset[j];
        int nearest_centroid_id = 0;
        double nearest_centroid_dis = HUGE_VAL;

        for (int i = 0; i < k; i++) {
            double dis = distance1(data_point, centroid_list[i]->getCoordinate());

            if (dis >= nearest_centroid_dis)
                continue;
            nearest_centroid_dis = dis;
            nearest_centroid_id = i;
        }

        labels[j] = nearest_centroid_id;
        centroid_list[nearest_centroid_id]->getCluster()->addDataId(j);
    }
}

void Lloyd::updateCentroids() {
    for (auto centroid : centroid_list) {
        std::vector<int> data_id_list = centroid->getCluster()->getDataIdList();
        int data_num = data_id_list.size();

        std::vector<double> new_coordinate(data_dimension, 0);
        for (int data_id : data_id_list) {
            new_coordinate = addVector(new_coordinate, dataset[data_id]);
        }
        new_coordinate = divideVector(new_coordinate, data_num);

        centroid->updateCoordinate(new_coordinate);
    }
}