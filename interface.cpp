#include <iostream>
#include "experiment.h"
using namespace std;


int main(){
    Experiment* experiment = new Experiment();
    // experiment->test_Lloyd();
    // experiment->test_dask_means();
    // experiment->test_NoInB();
    // experiment->test_NoKnn();
    // experiment->test_NoBound();
    experiment->test_DualTree();
    return 0;
}