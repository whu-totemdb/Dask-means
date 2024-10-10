#include "algorithms/Lloyd.h"
#include "algorithms/DaskMeans.h"
#include "algorithms/NoInB.h"
#include "algorithms/NoKnn.h"
#include "algorithms/NoBound.h"
#include "algorithms/DualTree.h"
#include "structure/BallTree.h"

class Experiment {
public:
    Experiment() {}
    ~Experiment() {}

    void test_Lloyd();

    void test_dask_means();

    void test_NoInB();

    void test_NoKnn();

    void test_NoBound();

    void test_DualTree();
};

void Experiment::test_Lloyd() {
    std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/test.txt";
    // std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/ball-tree.txt";
    std::string output_path = "/home/lzp/cs/dask-means-cpp/output/lloyd_output.txt";

    Lloyd* lloyd = new Lloyd();
    // lloyd->initParameters(30, 2, 3);
    lloyd->initParameters(30, 2, 3);
    lloyd->load(data_path);
    // double start_time, end_time;
    // start_time = clock();
    lloyd->run();
    // end_time = clock();
    // cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << endl;
    lloyd->output(output_path);

    // delete lloyd;
}

void Experiment::test_dask_means() {
    std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/test.txt";
    // std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/ball-tree.txt";
    std::string output_path = "/home/lzp/cs/dask-means-cpp/output/dask_output.txt";


    DaskMeans* dask_means = new DaskMeans(2);
    dask_means->initParameters(30, 2, 3);
    dask_means->load(data_path);
    // double start_time, end_time;
    // start_time = clock();
    dask_means->run();
    // end_time = clock();
    // cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << endl;
    dask_means->output(output_path);

    delete dask_means;
}

void Experiment::test_NoInB() {
    std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/test.txt";
    // std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/ball-tree.txt";
    std::string output_path = "/home/lzp/cs/dask-means-cpp/output/NoInB_output.txt";

    NoInB* noInB = new NoInB(2);
    noInB->initParameters(30, 2, 3);
    noInB->load(data_path);
    // double start_time, end_time;
    // start_time = clock();
    noInB->run();
    // end_time = clock();
    // cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << endl;
    noInB->output(output_path);

    delete noInB;
}

void Experiment::test_NoKnn() {
    std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/test.txt";
    // std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/ball-tree.txt";
    std::string output_path = "/home/lzp/cs/dask-means-cpp/output/NoKnn_output.txt";

    NoKnn* noKnn = new NoKnn(2);
    noKnn->initParameters(30, 2, 3);
    noKnn->load(data_path);
    // double start_time, end_time;
    // start_time = clock();
    noKnn->run();
    // end_time = clock();
    // cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << endl;
    noKnn->output(output_path);

    delete noKnn;
}

void Experiment::test_NoBound() {
    MatrixOur dataset = load_data("/home/lzp/cs/dask-means-cpp/dataset/test.txt");
    
    double start_time, end_time;
    start_time = clock();
    VectorXi labels = ball_k_means(dataset, 3, true, true);
    end_time = clock();
    cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << endl;
}

void Experiment::test_DualTree() {
    std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/test.txt";
    // std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/ball-tree.txt";
    std::string output_path = "/home/lzp/cs/dask-means-cpp/output/DualTree_output.txt";

    DualTree* dual_tree = new DualTree(2);
    dual_tree->initParameters(30, 2, 3);
    dual_tree->load(data_path);
    // double start_time, end_time;
    // start_time = clock();
    dual_tree->run();
    // end_time = clock();
    // cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << endl;
    dual_tree->output(output_path);

    delete dual_tree;
}