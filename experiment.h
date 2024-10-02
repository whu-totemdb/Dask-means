#include "algorithms/Lloyd.h"
#include "algorithms/DaskMeans.h"
#include "structure/BallTree.h"

class Experiment {
public:
    Experiment() {}
    ~Experiment() {}

    void test_Lloyd();

    void test_ball_tree();
};

void Experiment::test_Lloyd() {
    std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/test.txt";
    std::string output_path = "/home/lzp/cs/dask-means-cpp/output/test_optput.txt";

    Lloyd* lloyd = new Lloyd();
    lloyd->initParameters(20, 2, 2);
    lloyd->load(data_path);
    lloyd->run();
    lloyd->output(output_path);

    delete lloyd;
}

void Experiment::test_ball_tree() {
    std::string data_path = "/home/lzp/cs/dask-means-cpp/dataset/ball-tree.txt";
    // std::string output_path = "/home/lzp/cs/dask-means-cpp/output/test_optput.txt";

    DaskMeans* dask_means = new DaskMeans();
    dask_means->initParameters(6, 2, 2);
    dask_means->load(data_path);
    dask_means->testKnn();

}

