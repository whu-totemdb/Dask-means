#include <iostream>
#include "experiment.h"
using namespace std;


int main(){
    // rewrite your dataset path here
    std::string data_path = "../dataset/1_2d_T_drive_11317142.txt";
    std::string output_path = "../output/T-drive-AAA.csv";

    int data_scale = 1000000;       // total data scale
    int data_dimension = 2;         // data dimension of the dataset
    std::vector<int> k_list(3);     // "k" list in k-means
    k_list[0] = 100;
    k_list[1] = 1000;
    k_list[2] = 10000;

    cout << "running: " << data_path << endl;

    for (auto k : k_list) {
        size_t pos = output_path.find("AAA");
        if (pos != std::string::npos) {
            output_path.replace(pos, 3, std::to_string(k));
        }
        cout << "======================Strating experiment with k=" << k << "======================" << endl;

        Experiment* experiment = new Experiment(data_scale, data_dimension, k);
        experiment->set_file_path(data_path, output_path);

        // you can select your algorithms here
        experiment->test_Lloyd();
        experiment->test_DualTree();
        experiment->test_NoBound();
        experiment->test_NoKnn();
        experiment->test_NoInB();
        experiment->test_dask_means();
        experiment->test_Hamerly();
        experiment->test_Yinyang();
        experiment->test_Elkan();
        experiment->test_Drake();

        output_path = "../output/T-drive-AAA.csv";
    }
} 