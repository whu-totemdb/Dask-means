#include "Drake.h"
#include "../utils/Utils.h"
#include <iostream>

using namespace Utils;

Drake::Drake(int max_iterations, double convergence_threshold)
    : KMeansBase(max_iterations, convergence_threshold) {
        b = k / 4;
        ub = std::vector<double>(data_scale, 0.0);
        a = std::vector<std::vector<KnnRes*>>(data_scale, std::vector<KnnRes*>());
        max_drift = 0.0;
        max_b = 0;
    }

void Drake::run() {
    int it = 0;
    initializeCentroids();
    for (size_t i = 0; i < data_scale; i++) {
        sortCentroids(i, b + 1, centroid_list);
    }

    do {
        assignLabels();
        updateCentroids();
    } while (!hasConverged() && it < max_iterations);
    
}

void Drake::assignLabels() {
    for (auto centroid : centroid_list) {
        centroid->cluster->clear();
    }

    for (int j = 0; j < data_scale; j++) {
        if (labels[j] != 1) {
            ub[j] += centroid_list[a[j][0]->id]->drift;
            for (int i = 1; i < b + 1; i++) {
                a[j][i - 1]->dis -= centroid_list[a[j][i]->id]->drift;
            }
        }
        bool flag = false;
        for (int z = 1; z < b + 1; z++) {
            if (ub[j] <= a[j][z - 1]->dis) {
                sortCentroids(j, z + 1, centroid_list, a[j]);
                centroid_list[a[j][0]->id]->cluster->addDataId(j);
                labels[j] = a[j][0]->id;
                max_b = std::max(max_b, z);
                flag = true;
                break;
            }
        }
        if (!flag) {
            sortCentroids(j, b + 1, centroid_list);
            centroid_list[a[j][0]->id]->cluster->addDataId(j);
            labels[j] = a[j][0]->id;
        }
    }
}

void Drake::updateCentroids() {
    max_drift = 0.0;
    for (auto centroid : centroid_list) {
        std::vector<int> data_id_list = centroid->cluster->getDataIdList();
        int data_num = data_id_list.size();

        std::vector<double> new_coordinate(data_dimension, 0);
        for (int data_id : data_id_list) {
            new_coordinate = addVector(new_coordinate, dataset[data_id]);
        }
        if (data_num != 0) {
            new_coordinate = divideVector(new_coordinate, data_num);
            centroid->updateCoordinate(new_coordinate);
        }

        max_drift = std::max(max_drift, centroid->max_drift);
    }
}

// given point_id, sort and get q nearest centroids
void Drake::sortCentroids(int point_id, int q, std::vector<Centroid*> cent_list) {
    a[point_id].clear();

    std::vector<double> point = dataset[point_id];
    std::sort(cent_list.begin(), cent_list.end(), [this, point](const Centroid* a, const Centroid* b) {
        double dist_a = distance1(point, a->coordinate);
        double dist_b = distance1(point, b->coordinate);
        return dist_a < dist_b;
    });

    ub[point_id] = distance1(cent_list[0]->coordinate, point);
    for (size_t i = 1; i < q; i++) {
        KnnRes* nearest_cent = new KnnRes();
        nearest_cent->dis = distance1(cent_list[i]->coordinate, point);
        nearest_cent->id = cent_list[i]->centroid_id;
        a[point_id].push_back(nearest_cent);
    }
}

void Drake::sortCentroids(int point_id, int q, std::vector<Centroid*> cent_list,
        std::vector<KnnRes*> res) {
    a[point_id].clear();

    std::vector<double> point = dataset[point_id];
    std::sort(res.begin(), res.end(), [this, cent_list, point](const KnnRes* a, const KnnRes* b) {
        double dist_a = distance1(point, cent_list[a->id]->coordinate);
        double dist_b = distance1(point, cent_list[b->id]->coordinate);
        return dist_a < dist_b;
    });

    ub[point_id] = distance1(cent_list[res[0]->id]->coordinate, point);
    for (size_t i = 1; i < q; i++) {
        KnnRes* nearest_cent = new KnnRes();
        nearest_cent->dis = distance1(cent_list[res[i]->id]->coordinate, point);
        nearest_cent->id = res[i]->id;
        a[point_id].push_back(nearest_cent);
    }
}