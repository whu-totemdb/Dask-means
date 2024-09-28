#include <iostream>
#include "experiment.h"
using namespace std;


int main(){
    Experiment* experiment = new Experiment();
    experiment->test_Lloyd();
    return 0;
}