#include "NoInB.h"
#include <iostream>

NoInB::NoInB(int capacity, int max_iterations, double convergence_threshold)
    : DaskMeans(capacity, max_iterations, convergence_threshold) {}

NoInB::~NoInB() {}

void NoInB::run() {
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
    std::cout << "successfully run NoInB in " << total_runtime << " s" << std::endl;

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
        if (node.leftChild != nullptr) {
            assignLabels(*node.leftChild, res[1]->dis + node.radius);
        }
        if (node.rightChild != nullptr) {
            assignLabels(*node.rightChild, res[1]->dis + node.radius);
        }
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

            delete res;
        }
    }

    for (int j = 0; j < 2; j++) {
        delete res[j];
    }
}

void NoInB::assignToCluster(Node& node, int centroid_id) {
    if (node.centroid_id != -1) {
        Cluster* old_cluster = centroid_list[node.centroid_id]->cluster;
        old_cluster->dataOut(node.sum_vector, &node);
    }

    node.centroid_id = centroid_id;
    if (node.leaf) {
        for (int i = 0; i < node.point_number; i++) {
            int clu_id = node.centroid_id_for_data[i];
            if (clu_id != -1) {
                Cluster* old_cluster = centroid_list[clu_id]->cluster;
                old_cluster->dataOut(dataset[node.data_id_list[i]], node.data_id_list[i]);
            }
            node.centroid_id_for_data[i] = centroid_id;
        }
    } else {
        if (node.leftChild != nullptr) {
            assignToCluster(*node.leftChild, centroid_id);
        }
        if (node.rightChild != nullptr) {
            assignToCluster(*node.rightChild, centroid_id);
        }
    }
}