#include "NoKnn.h"
#include <iostream>

NoKnn::NoKnn(int capacity, int max_iterations, double convergence_threshold)
    : DaskMeans(capacity, max_iterations, convergence_threshold) {}

NoKnn::~NoKnn() {}

void NoKnn::run() {
    int it = 0;     // iteration
    double start_time, end_time;
    start_time = clock();

    buildDataIndex(this->capacity);
    initializeCentroids();

    end_time = clock();
    init_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "build index and initialize in " << init_time << " s" << std::endl;

    // main loop
    do {
        start_time = clock();

        buildCentroidIndex();
        if (it == 0) {
            ub = std::vector<double>(k, std::numeric_limits<double>::max());
        } else {
            for (int i = 0; i < k; i++) {
                ub[i] = inner_bound[i] + centroid_list[i]->drift + centroid_list[i]->max_drift;
            }
        }
        setInnerBound();
        assignLabels(*data_index->root, std::numeric_limits<double>::max());
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
    std::cout << "successfully run NoKnn in " << total_runtime << " s" << std::endl;

}

void NoKnn::setInnerBound() {
    // using simple distance calculation to set inner bound
    this->inner_bound = std::vector<double>(k, -1.0);
    this->inner_id = std::vector<double>(k, -1.0);
    for (int i = 0; i < k; i++) {
        std::vector<KnnRes*> res(2);
        for (int j = 0; j < 2; j++) {
            res[j] = new KnnRes(ub[i]);
        }
        ballTree2nn(centroid_list[i]->coordinate, *(centroid_index->root), res, centroid_list);
        inner_bound[i] = res[1]->dis;
        inner_id[i] = res[1]->id;

        for (int j = 0; j < 2; j++) {
            delete res[j];
        }
    }
}

void NoKnn::assignLabels(Node& node, double ub) {
    // 1. if the node is assigned before (pruning 1)
    if (node.centroid_id != -1  && 
        distance1(node.pivot, centroid_list[node.centroid_id]->coordinate) 
        + node.radius < inner_bound[node.centroid_id] / 2) 
        { return; }

    // 2. find two nearest centroid for the node (pruning 2)
    std::vector<KnnRes*> res(2);
    for (int i = 0; i < 2; i++) {
        res[i] = new KnnRes(ub);
    }
    calculate2nn(node.pivot, res, centroid_list);
    if (res[1]->dis - res[0]->dis > 2 * node.radius) {
        if (res[0]->id == node.centroid_id) {
            return;
        }
        assignToCluster(node, res[0]->id);
        Cluster* new_cluster = centroid_list[node.centroid_id]->cluster;
        new_cluster->dataIn(node.point_number, node.sum_vector);
        return;
    }

    if (!node.leaf) {
        // 3. split the node into two child node
        assignToCluster(node, -1);
        assignLabels(*node.leftChild, res[1]->dis + node.radius);
        assignLabels(*node.rightChild, res[1]->dis + node.radius);
    } else {
        if (node.centroid_id != -1) {
            Cluster* old_cluster = centroid_list[node.centroid_id]->cluster;
            old_cluster->dataOut(node.point_number, node.sum_vector);
        }
        // 4. assign each point in leaf node
        for (int i = 0; i < node.point_number; i++) {
            std::vector<double> data = dataset[node.data_id_list[i]];
            int centroid_id = node.centroid_id_for_data[i];
            // if the point is assigned before
            if (centroid_id != -1 && distance1(data, centroid_list[centroid_id]->coordinate) 
                < inner_bound[centroid_id] / 2 )
                { return; }

            // use 1nn
            KnnRes* res = new KnnRes(ub);
            calculate1nn(dataset[node.data_id_list[i]], *res, centroid_list);
            if (centroid_id != -1) {
                Cluster* old_cluster = centroid_list[centroid_id]->cluster;
                old_cluster->dataOut(1, data);
            }
            centroid_id = res[0].id;
            Cluster* new_cluster = centroid_list[centroid_id]->cluster;
            new_cluster->dataIn(1, data);

            delete res;
        }
    }

    for (int j = 0; j < 2; j++) {
        delete res[j];
    }
}