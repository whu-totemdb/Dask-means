#include "algorithms/Lloyd.h"
#include "algorithms/DaskMeans.h"
#include "algorithms/NoInB.h"
#include "algorithms/NoKnn.h"
#include "algorithms/NoBound.h"
#include "algorithms/DualTree.h"
#include "algorithms/Hamerly.h"
#include "algorithms/Drake.h"
#include "algorithms/Yinyang.h"
#include "structure/BallTree.h"

class Experiment {
protected:
    std::string data_path;
    std::string output_path;

    int leaf_capacity = 30;
    int data_scale;
    int data_dimension;
    int k;

public:
    Experiment() {}
    Experiment(int data_scale, int data_dimension, int k)
        : data_scale(data_scale), data_dimension(data_dimension), k(k) {}
    ~Experiment() {}

    void set_file_path(const std::string& data_path, const std::string& output_path);

    void test_Lloyd();

    void test_dask_means();

    void test_NoInB();

    void test_NoKnn();

    void test_NoBound();

    void test_DualTree();

    void test_Hamerly();

    void test_Drake();

    void test_Yinyang();
};

void Experiment::set_file_path(const std::string& data_path, const std::string& output_path) {
    this->data_path = data_path;
    this->output_path = output_path;
}

void Experiment::test_Lloyd() {
    cout << "=============starting Lloyd=============" << endl;
    Lloyd* lloyd = new Lloyd();
    lloyd->initParameters(data_scale, data_dimension, k);
    lloyd->load(data_path);
    lloyd->run();
    // lloyd->output(output_path);
    lloyd->writeRuntime(output_path);
    delete lloyd;
}

void Experiment::test_dask_means() {
    cout << "=============starting Dask-means=============" << endl;
    DaskMeans* dask_means = new DaskMeans(leaf_capacity);
    dask_means->initParameters(data_scale, data_dimension, k);
    dask_means->load(data_path);
    dask_means->run();
    // dask_means->output(output_path);
    dask_means->writeRuntime(output_path);
    delete dask_means;
}

void Experiment::test_NoInB() {
    cout << "=============starting NoInB=============" << endl;
    NoInB* noInB = new NoInB(leaf_capacity);
    noInB->initParameters(data_scale, data_dimension, k);
    noInB->load(data_path);
    noInB->run();
    // noInB->output(output_path);
    noInB->writeRuntime(output_path);
    delete noInB;
}

void Experiment::test_NoKnn() {
    cout << "=============starting NoKnn=============" << endl;
    NoKnn* noKnn = new NoKnn(leaf_capacity);
    noKnn->initParameters(data_scale, data_dimension, k);
    noKnn->load(data_path);
    noKnn->run();
    // noKnn->output(output_path);
    noKnn->writeRuntime(output_path);
    delete noKnn;
}

void Experiment::test_NoBound() {
    cout << "=============starting NoBound=============" << endl;
    NoBound* no_bound = new NoBound();
    no_bound->initParameters(data_scale, data_dimension, k);
    no_bound->run(data_path.c_str());
    no_bound->writeRuntime(output_path);
    delete no_bound;
}

void Experiment::test_DualTree() {
    cout << "=============starting DualTree=============" << endl;
    DualTree* dual_tree = new DualTree(leaf_capacity);
    dual_tree->initParameters(data_scale, data_dimension, k);
    dual_tree->load(data_path);
    dual_tree->run();
    // dual_tree->output(output_path);
    dual_tree->writeRuntime(output_path);
    delete dual_tree;
}

void Experiment::test_Hamerly() {
    cout << "=============starting Hamerly=============" << endl;
    Hamerly* hamerly = new Hamerly();
    hamerly->initParameters(data_scale, data_dimension, k);
    hamerly->load(data_path);
    hamerly->run();
    hamerly->writeRuntime(output_path);
    delete hamerly;
}

void Experiment::test_Drake() {
    cout << "=============starting Drake=============" << endl;
    Drake* drake = new Drake();
    drake->initParameters(data_scale, data_dimension, k);
    drake->load(data_path);
    drake->run();
    drake->writeRuntime(output_path);
    delete drake;
}

void Experiment::test_Yinyang() {
    cout << "=============starting Yinyang=============" << endl;
    Yinyang* yinyang = new Yinyang();
    yinyang->initParameters(data_scale, data_dimension, k);
    yinyang->load(data_path);
    yinyang->run();
    yinyang->writeRuntime(output_path);
    delete yinyang;
}