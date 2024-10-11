#include "NoInB.h"
#include <iostream>

NoInB::NoInB(int capacity, int max_iterations, double convergence_threshold)
    : DaskMeans(capacity, max_iterations, convergence_threshold) {}

NoInB::~NoInB() {}

void NoInB::run() {
    int it = 0;     // iteration
    double start_time, end_time;
    start_time = clock();

    // main loop
    buildDataIndex(this->capacity);
    initializeCentroids();
    do {
        if (it != 0) {
            start_time = clock();
        }
        buildCentroidIndex();
        assignLabels(*data_index->root, std::numeric_limits<double>::max());
        updateCentroids();
        // output("/home/lzp/cs/dask-means-cpp/output/NoInB_output.txt");

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
    std::cout << "successfully run NoInB in " << total_runtime << " ms" << std::endl;

}

void NoInB::assignLabels(Node& node, double ub) {
    std::vector<KnnRes*> res(2);
    for (int i = 0; i < 2; i++) {
        res[i] = new KnnRes(ub);
    }
    ballTree2nn(node.pivot, *(centroid_index->root), res, centroid_list);
    if (res[1]->dis - res[0]->dis > 2 * node.radius) {
        assignToCluster(node, res[0]->id);
        Cluster* new_cluster = centroid_list[node.centroid_id]->getCluster();
        new_cluster->dataIn(node.sum_vector, &node);
        return;
    }

    if (!node.isLeaf()) {
        // 3. split the node into two child node
        assignToCluster(node, -1);
        assignLabels(*node.leftChild, res[1]->dis + node.radius);
        assignLabels(*node.rightChild, res[1]->dis + node.radius);
    } else {
        if (node.centroid_id != -1) {
            Cluster* old_cluster = centroid_list[node.centroid_id]->getCluster();
            old_cluster->dataOut(node.sum_vector, &node);
        }
        // 4. assign each point in leaf node
        for (int i = 0; i < node.point_number; i++) {          
            // use 1nn
            KnnRes* res = new KnnRes(ub);
            ballTree1nn(dataset[node.data_id_list[i]], *(centroid_index->root), *res, centroid_list);
            if (node.centroid_id_for_data[i] != -1) {
                Cluster* old_cluster = centroid_list[node.centroid_id_for_data[i]]->getCluster();
                old_cluster->dataOut(dataset[node.data_id_list[i]], node.data_id_list[i]);
            }
            node.centroid_id_for_data[i] = res[0].id;
            Cluster* new_cluster = centroid_list[node.centroid_id_for_data[i]]->getCluster();
            new_cluster->dataIn(dataset[node.data_id_list[i]], node.data_id_list[i]);
        }
    }
}