#include "Elkan.h"
#include "../utils/Utils.h"
#include "../structure/KnnRes.h"
#include <iostream>

using namespace Utils;

Elkan::Elkan(int max_iterations, double convergence_threshold)
    : KMeansBase(max_iterations, convergence_threshold) {}

void Elkan::initParameters(int data_scale, int data_dimension, int k) {
    this->data_scale = data_scale;
    this->data_dimension = data_dimension;
    this->k = k;
    this->labels.assign(this->data_scale, -1);
    ub = std::vector<double>(data_scale, 0.0);
    lb = std::vector<std::vector<double>>(data_scale, std::vector<double>(k, 0.0));
}

void Elkan::run() {
    int it = 0;     // iteration
    double start_time, end_time;
    start_time = clock();
    initializeCentroids();
    end_time = clock();
    init_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "initialize centroids in " << init_time << " s" << std::endl;

    // main loop
    do {
        start_time = clock();
        setInnerBound();
        if (it == 0) {
            assignAndSetBound();
        } else {
            assignLabels();
        }

        updateCentroids();

        end_time = clock();
        runtime[it] = double(end_time - start_time) / CLOCKS_PER_SEC;
        std::cout << "iter: " << it << ", runtime: " << runtime[it] << " s" << std::endl;
        it++;
    } while (!hasConverged() && it < max_iterations);

    // show total runtime
    double total_runtime = init_time;
    for (size_t i = 0; i < max_iterations; i++) {
        total_runtime += runtime[i];
    }
    std::cout << "successfully run Elkan in " << total_runtime << " s" << std::endl;

}

void Elkan::assignLabels() {
    // clear all clusters firstly
    for (auto centroid : centroid_list) {
        centroid->cluster->clear();
    }

    for (int j = 0; j < data_scale; j++) {
        bool ub_reset = false;

        auto centroid = centroid_list[labels[j]];
        int centroid_id = labels[j];
        // 1.update ub and lb
        ub[j] += centroid->drift;
        for (int i = 0; i < k; i++) {
            lb[j][i] -= centroid_list[i]->drift;
        }
        if (ub[j] <= inner_bound[centroid_id] / 2) {
            centroid->cluster->addDataId(j);
            continue;
        }
        
        // 2.assign all data point to the closest ccentroid
        auto data_point = dataset[j];
        int nearest_centroid_id = centroid_id;
        double nearest_centroid_dis = HUGE_VAL;

        for (int i = 0; i < k; i++) {
            if (i == labels[j] || ub[j] <= lb[j][i]) {
                continue;
            }

            // 2.1.reset lower bound
            double dis = distance1(data_point, centroid_list[i]->coordinate);
            lb[j][i] = dis;
            if (ub[j] <= lb[j][i]) {
                continue;
            }

            // 2.2.reset upper bound
            if (!ub_reset) {
                double dis1 = distance1(data_point, centroid_list[centroid_id]->coordinate);
                ub[j] = dis1;
                ub_reset = true;
                if (ub[j] <= lb[j][i]) {
                    continue;
                }
            }

            ub[j] = dis;
            nearest_centroid_id = i;
        }

        labels[j] = nearest_centroid_id;
        centroid_list[nearest_centroid_id]->cluster->addDataId(j);
    }
}

void Elkan::setInnerBound() {
    // using index ball-tree knn to set inner bound
    this->inner_bound = std::vector<double>(k, -1.0);
    for (int i = 0; i < k; i++) {
        std::vector<KnnRes*> res(2);
        for (int j = 0; j < 2; j++) {
            res[j] = new KnnRes();
        }
        calculate2nn(centroid_list[i]->coordinate, res, centroid_list);
        inner_bound[i] = res[1]->dis;
        
        for (int j = 0; j < 2; j++) {
            delete res[j];
        }
    }
}

void Elkan::assignAndSetBound() {
    for (int j = 0; j < data_scale; j++) {
        auto data_point = dataset[j];
        int nearest_centroid_id = 0;
        double nearest_centroid_dis = HUGE_VAL;

        for (int i = 0; i < k; i++) {
            double dis = distance1(data_point, centroid_list[i]->getCoordinate());
            lb[j][i] = dis;

            if (dis >= nearest_centroid_dis)
                continue;
            nearest_centroid_dis = dis;
            nearest_centroid_id = i;
        }

        ub[j] = nearest_centroid_dis;
        labels[j] = nearest_centroid_id;
        centroid_list[nearest_centroid_id]->getCluster()->addDataId(j);
    }
}
