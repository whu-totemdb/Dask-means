#include "algorithms/Lloyd.h"
#include "algorithms/DaskMeans.h"
#include "algorithms/NoInB.h"
#include "algorithms/NoKnn.h"
#include "algorithms/NoBound.h"
#include "algorithms/DualTree.h"
#include "structure/BallTree.h"

class Experiment {
protected:
    std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/test.txt";
    std::string output_path = "/home/lzp/cs/dask-means-cpp/output/lloyd_output.txt";

    int leaf_capacity = 2;
    int data_scale;
    int data_dimension;
    int k;

public:
    Experiment() {}
    Experiment(int data_scale, int data_dimension, int k)
        : data_scale(data_scale), data_dimension(data_dimension), k(k) {}
    ~Experiment() {}

    void test_Lloyd();

    void test_dask_means();

    void test_NoInB();

    void test_NoKnn();

    void test_NoBound();

    void test_DualTree();
};

void Experiment::test_Lloyd() {
    cout << "=============starting Lloyd=============" << endl;
    Lloyd* lloyd = new Lloyd();
    lloyd->initParameters(data_scale, data_dimension, k);
    lloyd->load(data_path);
    lloyd->run();
    // lloyd->output(output_path);
    delete lloyd;
}

void Experiment::test_dask_means() {
    cout << "=============starting Dask-means=============" << endl;
    DaskMeans* dask_means = new DaskMeans(leaf_capacity);
    dask_means->initParameters(data_scale, data_dimension, k);
    dask_means->load(data_path);
    dask_means->run();
    // dask_means->output(output_path);
    delete dask_means;
}

void Experiment::test_NoInB() {
    cout << "=============starting NoInB=============" << endl;
    NoInB* noInB = new NoInB(leaf_capacity);
    noInB->initParameters(data_scale, data_dimension, k);
    noInB->load(data_path);
    noInB->run();
    // noInB->output(output_path);
    delete noInB;
}

void Experiment::test_NoKnn() {
    cout << "=============starting NoKnn=============" << endl;
    NoKnn* noKnn = new NoKnn(leaf_capacity);
    noKnn->initParameters(data_scale, data_dimension, k);
    noKnn->load(data_path);
    noKnn->run();
    // noKnn->output(output_path);
    delete noKnn;
}

void Experiment::test_NoBound() {
    cout << "=============starting NoBound=============" << endl;
    NoBound* no_bound = new NoBound();
    no_bound->initParameters(data_scale, data_dimension, k);
    no_bound->run(data_path.c_str());
    delete no_bound;
}

void Experiment::test_DualTree() {
    cout << "=============starting DualTree=============" << endl;
    DualTree* dual_tree = new DualTree(leaf_capacity);
    dual_tree->initParameters(data_scale, data_dimension, k);
    dual_tree->load(data_path);
    dual_tree->run();
    // dual_tree->output(output_path);
    delete dual_tree;
}