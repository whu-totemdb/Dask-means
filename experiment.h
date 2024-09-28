#include "algorithms/Lloyd.h"

class Experiment {
public:
    Experiment() {}
    ~Experiment() {}

    void test_Lloyd();
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

