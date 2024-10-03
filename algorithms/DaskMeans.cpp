#include "DaskMeans.h"
#include "../structure/KnnRes.h"
#include "../structure/Node.h"
#include "../utils/Utils.h"

using namespace Utils;

DaskMeans::DaskMeans(int max_iterations, double convergence_threshold)
    : KMeansBase(max_iterations, convergence_threshold) {
    
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

void DaskMeans::initInnerBound() {
    // using index ball-tree knn to initial inner bound
    inner_bound = std::vector<double>(k, -1.0);
    for (int i = 0; i < k; i++) {
        if (inner_bound[i] >= 0) {
            continue;
        }
        std::vector<KnnRes*> res(2);
        for (int i = 0; i < 2; i++) {
            res[i] = new KnnRes();
        }
        ballTree2nn(centroid_list[i]->getCoordinate(), *(centroid_index->root), res, centroid_list);
        inner_bound[i] = res[1]->dis;
        inner_bound[res[1]->id] = res[1]->dis;
    }

}

void DaskMeans::assignLabels(Node& node, double ub) {
    // 1. if the node is assigned before (pruning 1)
    if (node.centroid_id != -1  && 
        distance1(node.pivot, centroid_list[node.centroid_id]->getCoordinate()) 
        + node.radius < inner_bound[node.centroid_id] / 2) 
        { return; }

    // 2. find two nearest centroid for the node (pruning 2)
    std::vector<KnnRes*> res(2);
    for (int i = 0; i < 2; i++) {
        res[i] = new KnnRes(ub);
    }
    ballTree2nn(node.pivot, *(centroid_index->root), res, centroid_list);
    if (res[1]->dis - res[0]->dis > 2 * node.radius) {
        node.setAssigned(res[0]->id);
        return;
    }

    if (!node.isLeaf()) {
        // 3. split the node into two child node
        assignLabels(*node.leftChild, res[1]->dis + node.radius);
        assignLabels(*node.rightChild, res[1]->dis + node.radius);
    } else {
        // 4. assign each point in leaf node
        for (int i = 0; i < node.point_number; i++) {
            // if the point is assigned before
            if (node.centroid_id_for_data[i] != -1 && distance1(dataset[node.data_id_list[i]], 
                centroid_list[node.centroid_id_for_data[i]]->getCoordinate()) 
                < inner_bound[node.centroid_id_for_data[i]] / 2 )
                { return; }
            
            // use 1nn
            KnnRes* res = new KnnRes(ub);
            ballTree1nn(dataset[node.data_id_list[i]], *(centroid_index->root), *res, centroid_list);
            node.centroid_id_for_data[i] = res[0].id;
        }
    }
}