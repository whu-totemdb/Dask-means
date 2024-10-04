#include <iostream>
#include "experiment.h"
using namespace std;


int main(){
    Experiment* experiment = new Experiment();
    experiment->test_Lloyd();
    experiment->test_dask_means();
    return 0;
}