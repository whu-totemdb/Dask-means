// DaskMeans.h
#ifndef DASKMEANS_H
#define DASKMEANS_H

#include <vector>
#include "KMeansBase.h"
#include "../utils/Utils.h"
#include <../structure/BallTree.h>

class DaskMeans : public KMeansBase {
public:
    BallTree* ballTree;

    DaskMeans();
    ~DaskMeans();

    void buildBallTree(){
        ballTree->buildBallTree(dataset, 6);
    }

    void run() {};
    
};

DaskMeans::DaskMeans() {
    ballTree = new BallTree();
}

DaskMeans::~DaskMeans()
{
}


#endif